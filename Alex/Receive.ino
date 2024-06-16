TResult readPacket(TPacket *packet) 
{
  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  return len == 0 ? PACKET_INCOMPLETE : deserialize(buffer, len, packet);
}

void handleResult(TResult result,TPacket *recvPacket)
{
  switch(result)
  { 
    case PACKET_OK:
      handlePacket(recvPacket);
    break;

    case PACKET_BAD:
      sendBadPacket();
    break;

    case PACKET_CHECKSUM_BAD:
      sendBadChecksum();
    break; 

  } 
}

void handlePacket(TPacket *packet)
{
  switch(packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
    break;

    case PACKET_TYPE_RESPONSE:
    break;

    case PACKET_TYPE_ERROR:
    break;

    case PACKET_TYPE_MESSAGE:
    break;

    case PACKET_TYPE_HELLO:
    break;
  }
}

void handleCommand(TPacket *command)
{
  switch(command->command) 
  {

    case COMMAND_FORWARD:
      forward(command->params[0]);
      sendOK();
    break;

    case COMMAND_REVERSE:
      backward(command->params[0]);
      sendOK();
    break;

    case COMMAND_TURN_LEFT:
      left(command->params[0]);
      sendOK();
    break;

    case COMMAND_TURN_RIGHT:
      right(command->params[0]);
      sendOK();
    break;

    case COMMAND_STOP:
      stop();
      sendOK();
    break;

    case MOVE_DIST_UP:
      currMoveTime += MOVE_DELTA;
      sendUpdate();
    break;

    case MOVE_DIST_DOWN:
      currMoveTime -= MOVE_DELTA;
      sendUpdate();
    break;

    case TURN_DIST_UP:
      currTurnTime += TURN_DELTA;
      sendUpdate();
    break;

    case TURN_DIST_DOWN:
      currTurnTime -= TURN_DELTA;
      sendUpdate();
    break;

    case MOVE_POWER_UP:
      currMoveSpeed = min(100, currMoveSpeed + POWER_DELTA);
      sendUpdate();
    break;

    case MOVE_POWER_DOWN:
      currMoveSpeed = max(0, currMoveSpeed - POWER_DELTA);
      sendUpdate();
    break;

    case TURN_POWER_UP:
      currTurnSpeed = min(100, currTurnSpeed + POWER_DELTA);
      sendUpdate();
    break;

    case TURN_POWER_DOWN:
      currTurnSpeed = max(0, currTurnSpeed - POWER_DELTA);
      sendUpdate();
    break;

    case COMMAND_GET_STATS:
      sendStatus();
    break;

    case COMMAND_CLEAR_STATS:
      clearCounters();
      sendOK();
    break;

    case COMMAND_COLOUR:
      sendColour();
    break;

    case COMMAND_ULTRASONIC:
      sendDist();
    break;

    default:
      sendBadCommand();
    break;
  }
}
