#include <serialize.h>
#include "packet.h"
#include "constants.h"
#include <stdarg.h>
#include <math.h>
//Alex's state variables

volatile TDirection dir = STOP;

unsigned long targetTime = 0;

unsigned long currMoveTime = MOVE_NORMAL;
unsigned long currTurnTime = TURN_NORMAL;

volatile float currMoveSpeed = MOVE_SPEED;
volatile float currTurnSpeed = TURN_SPEED;

void setup() 
{
  cli();
  setupEINT();
  setupSerial();
  startSerial();
  enablePullups();
  clearCounters();
  sei();
}

void loop() 
{
  TPacket recvPacket; // This holds commands from the Pi
  TResult result = readPacket(&recvPacket); //copies data from buffer to recvPacket, returns TResult
  handleResult(result,&recvPacket); //initiates Receive chain

  if (dir != STOP && (millis() > targetTime)) stop();
}
