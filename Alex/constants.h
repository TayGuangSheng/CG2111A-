#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */

//Alex Dimensions
const int ALEX_LENGTH  = 34;
const int ALEX_BREADTH = 16;
const int COUNTS_PER_REV =  1;
const int WHEEL_CIRC = 7;
const float ALEX_DIAGONAL = sqrt(ALEX_LENGTH*ALEX_LENGTH + ALEX_BREADTH*ALEX_BREADTH);
const float ALEX_CIRC = PI * ALEX_DIAGONAL;

//Movement Constants
const unsigned long  MOVE_SMALL = 100;
const unsigned long  MOVE_NORMAL = 200;
const unsigned long  MOVE_LARGE = 400;
const unsigned long  MOVE_DELTA = 100;
const float          MOVE_SPEED = 80;

const unsigned long  TURN_SMALL = 100;
const unsigned long  TURN_NORMAL = 200;
const unsigned long  TURN_LARGE = 400;
const unsigned long  TURN_DELTA = 100;
const unsigned long  POWER_DELTA = 10;
const float          TURN_SPEED = 100;

// Packet types
typedef enum
{
  PACKET_TYPE_COMMAND = 0,
  PACKET_TYPE_RESPONSE = 1,
  PACKET_TYPE_ERROR = 2,
  PACKET_TYPE_MESSAGE = 3,
  PACKET_TYPE_HELLO = 4,
} TPacketType;

// Response types. This goes into the command field
typedef enum
{
  RESP_OK = 0,
  RESP_STATUS=1,
  RESP_BAD_PACKET = 2,
  RESP_BAD_CHECKSUM = 3,
  RESP_BAD_COMMAND = 4, 
  RESP_BAD_RESPONSE = 5,
  RESP_COLOR = 6,
  RESP_DIST = 7, 
  RESP_UPDATE
} TResponseType;


// Commands
// For direction commands, param[0] = distance in cm to move param[1] = speed
typedef enum
{
  COMMAND_FORWARD = 0,
  COMMAND_REVERSE = 1,
  COMMAND_TURN_LEFT = 2,
  COMMAND_TURN_RIGHT = 3,
  COMMAND_STOP = 4,
  COMMAND_GET_STATS = 5,
  COMMAND_CLEAR_STATS = 6,
  COMMAND_ULTRASONIC = 7,
  COMMAND_COLOUR = 8,
  MOVE_DIST_UP,
  MOVE_DIST_DOWN,
  TURN_DIST_UP,
  TURN_DIST_DOWN,
  MOVE_POWER_UP,
  MOVE_POWER_DOWN,
  TURN_POWER_UP,
  TURN_POWER_DOWN,
} TCommandType;

typedef enum
{
  FWD = 0,
  BWD = 1,
  LEFT = 2,
  RIGHT = 3,
  STOP = 4,
} TDirection;


#endif
