#include <stdio.h>
#include <stdint.h>  
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "packet.h"
#include "serial.h"
#include "serialize.h"
#include "constants.h"
#include <unistd.h>
#include <termios.h>


#define PORT_NAME			"/dev/ttyACM0"
#define BAUD_RATE			B9600

int exitFlag = 0;
int busyFlag = 0;
int manual = 0;
sem_t _xmitSema;



void handleError(TResult error)
{
	switch (error)
	{
	case PACKET_BAD:
		printf("ERROR: Bad Magic Number\n");
		break;

	case PACKET_CHECKSUM_BAD:
		printf("ERROR: Bad checksum\n");
		break;

	default:
		printf("ERROR: UNKNOWN ERROR\n");
	}
}

void handleStatus(TPacket* packet)
{
	printf("\n ------- ALEX STATUS REPORT ------- \n\n");
	printf("Left Forward Ticks:\t\t%d\n", packet->params[0]);
	printf("Right Forward Ticks:\t\t%d\n", packet->params[1]);
	printf("Left Reverse Ticks:\t\t%d\n", packet->params[2]);
	printf("Right Reverse Ticks:\t\t%d\n", packet->params[3]);
	printf("Left Forward Ticks Turns:\t%d\n", packet->params[4]);
	printf("Right Forward Ticks Turns:\t%d\n", packet->params[5]);
	printf("Left Reverse Ticks Turns:\t%d\n", packet->params[6]);
	printf("Right Reverse Ticks Turns:\t%d\n", packet->params[7]);
	printf("Forward Distance:\t\t%d\n", packet->params[8]);
	printf("Reverse Distance:\t\t%d\n", packet->params[9]);
	printf("\n---------------------------------------\n\n");
}

void handleSensor(TPacket* packet){
	printf("\nDistance:\t\t%d cm\n\n", packet ->params[0]);
}

void handleColour(TPacket* packet) {
	uint32_t red = packet->params[0];
	uint32_t green = packet->params[1];
	uint32_t blue = packet->params[2];
	uint32_t distance = packet->params[3];

	printf("\n --------- Colour Sensor --------- \n\n");
	printf("R:\t%d\n", red);
	printf("G:\t%d\n", green);
	printf("B:\t%d\n", blue);
	printf("Distance:\t\t%d cm\n\n", distance);

	if (distance<6 || distance>8){
		printf("Distance is wrong\n");
	}else if (red < 190 && blue < 190 && green < 190){
		printf("White\n");
	}else if (red < blue && red < green){
		printf("Red\n");
	}else if (green < red){
		printf("Green\n");
	}
}

void handleResponse(TPacket* packet)
{
	// The response code is stored in command
	switch (packet->command)
	{
	case RESP_OK:
		busyFlag = 0;
		printf("Command OK\n");
		break;

	case RESP_UPDATE:
		busyFlag = 0;
		printf("Move Time: %d\n Move Power: %d\n Turn Time: %d\n Turn Power: %d\n",
		packet->params[0],packet->params[1],packet->params[2],packet->params[3]);
		break;

	case RESP_STATUS:
		handleStatus(packet);
		busyFlag = 0;
		break;

	case RESP_COLOR:
		printf("received color command");
		handleColour(packet);
		busyFlag = 0;
		break;

	case RESP_DIST:
		printf("received ultrasonic command");
		handleSensor(packet);
		busyFlag = 0;
	break;

	default:
		printf("Arduino is confused\n");
	}
}

void handleErrorResponse(TPacket* packet)
{
	// The error code is returned in command
	switch (packet->command)
	{
	case RESP_BAD_PACKET:
		printf("Arduino received bad magic number\n");
		busyFlag = 0;
		break;

	case RESP_BAD_CHECKSUM:
		printf("Arduino received bad checksum\n");
		busyFlag = 0;
		break;

	case RESP_BAD_COMMAND:
		printf("Arduino received bad command\n");
		break;

	case RESP_BAD_RESPONSE:
		printf("Arduino received unexpected response\n");
		busyFlag = 0;
		break;

	default:
		printf("Arduino reports a weird error\n");
	}
}

void handleMessage(TPacket* packet)
{
	printf("Message from Alex: %s\n", packet->data);
}

void handlePacket(TPacket* packet)
{
	switch (packet->packetType)
	{
	case PACKET_TYPE_COMMAND:
		break;

	case PACKET_TYPE_RESPONSE:
		handleResponse(packet);
		break;

	case PACKET_TYPE_ERROR:
		handleErrorResponse(packet);
		break;

	case PACKET_TYPE_MESSAGE:
		handleMessage(packet);
		break;
	}
}

void sendPacket(TPacket* packet)
{
	char buffer[PACKET_SIZE];
	int len = serialize(buffer, packet, sizeof(TPacket));

	serialWrite(buffer, len);
}

void* receiveThread(void* p)
{
	char buffer[PACKET_SIZE];
	int len;
	TPacket packet;
	TResult result;
	int counter = 0;

	while (1)
	{
		len = serialRead(buffer);
		counter += len;
		if (len > 0)
		{
			result = deserialize(buffer, len, &packet);

			if (result == PACKET_OK)
			{
				counter = 0;
				handlePacket(&packet);
			}
			else
				if (result != PACKET_INCOMPLETE)
				{
					printf("PACKET ERROR\n");
					handleError(result);
				}
		}
	}
}

void flushInput()
{
	char c;

	while ((c = getchar()) != '\n' && c != EOF);
}

TCommandType charToCommand(char command){
	switch (command){
		case 'w': return COMMAND_FORWARD;
		case 's': return COMMAND_REVERSE;
		case 'd': return COMMAND_TURN_LEFT;
		case 'a': return COMMAND_TURN_RIGHT;
		case 'x': return COMMAND_STOP;
		case 'q': return COMMAND_COLOUR;
		case 'e': return COMMAND_ULTRASONIC;
		case 'z': return COMMAND_GET_STATS;
		case 'c': return COMMAND_CLEAR_STATS;
		case 'r': return MOVE_DIST_DOWN;
		case 't': return MOVE_DIST_UP;
		case 'f': return TURN_DIST_DOWN;
		case 'g': return TURN_DIST_UP;
		case 'y': return MOVE_POWER_DOWN;
		case 'u': return MOVE_POWER_UP;
		case 'h': return TURN_POWER_DOWN;
		case 'j': return TURN_POWER_UP;
		default : return COMMAND_BAD;
	}
}

void sendCommandPacket(char command, int distance){
	TCommandType commandType = charToCommand(command);

	if (commandType == COMMAND_BAD)
	{
		printf("\nUnrecognized Command.\n");
		busyFlag = 0;
		return;
	} 

	TPacket commandPacket;
	commandPacket.packetType = PACKET_TYPE_COMMAND;
	commandPacket.command = commandType;
	commandPacket.params[0] = distance;
	sendPacket(&commandPacket);
}

void sendCommand(char command, int distance)
{
	switch (command){ //handle non-arduino commands
		
		case 'i':
			system("play green.wav");
			busyFlag = 0;
		break;
		
		case 'k':
			system("play red.wav");
			busyFlag = 0;
		break;

		case 'm':
			system("play white.wav");
			busyFlag = 0;
		break;

		case 'v':
			exitFlag = 1;
			busyFlag = 0;
		break;	
		
		case 'm'
			manual = 1 - manual;
			busyFlag = 0;
		break;
		
		default:
			sendCommandPacket(command,distance); //if arduino command, send it a packet
		break;
	}
}

char getch() { // im gonna be real no clue how this works but it does :)
	char buf = 0;
	struct termios old = { 0 };
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
}

void getCommand()
{
	uint32_t distance;
	char ch;
	printf("\nWASD for movement, f=stop, e=get stats, r=clear stats, q=exit\n");
	while(!exitFlag) {
		distance = 0;

		if (manual) {
			printf("\nWASD for movement, f=stop, e=get stats, r=clear stats, q=exit\n");
			scanf("%c",&ch,&distance);
			if (ch != 'x') scanf("%d",&distance);
			flushInput(); 
		} else ch = getch(); 

		if (!busyFlag) {
			busyFlag = 1;
			printf("\nchr is %c\n",ch);
			sendCommand(ch, distance);
		}
	}
}


int main()
{
	// Connect to the Arduino
	startSerial(PORT_NAME, BAUD_RATE, 8, 'N', 1, 5);

	// Sleep for two seconds
	printf("WAITING TWO SECONDS FOR ARDUINO TO REBOOT\n");
	sleep(2);
	printf("DONE\n");

	// Spawn receiver thread
	pthread_t recv;

	pthread_create(&recv, NULL, receiveThread, NULL);

	// Send a hello packet
	TPacket helloPacket;

	helloPacket.packetType = PACKET_TYPE_HELLO;
	sendPacket(&helloPacket);

	getCommand();

	printf("Closing connection to Arduino.\n");
	endSerial();
}

