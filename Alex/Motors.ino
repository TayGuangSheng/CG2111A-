#include <AFMotor.h>
#define FRONT_LEFT   4 // M4 on the driver shield
#define FRONT_RIGHT  1 // M1 on the driver shield
#define BACK_LEFT    3 // M3 on the driver shield
#define BACK_RIGHT   2 // M2 on the driver shield

AF_DCMotor motorFL(FRONT_LEFT);
AF_DCMotor motorFR(FRONT_RIGHT);
AF_DCMotor motorBL(BACK_LEFT);
AF_DCMotor motorBR(BACK_RIGHT);

void setMotorSpeed(float speed)
{
  int speed_scaled = (speed/100.0) * 255;
  motorFL.setSpeed(speed_scaled);
  motorFR.setSpeed(speed_scaled);
  motorBL.setSpeed(speed_scaled);
  motorBR.setSpeed(speed_scaled);
}

void move(float power){

  setMotorSpeed(power);
  
  switch(dir)
  {
    case BWD:
      motorFL.run(BACKWARD);
      motorFR.run(BACKWARD);
      motorBL.run(FORWARD);
      motorBR.run(FORWARD); 
    break;
    
    case FWD:
      motorFL.run(FORWARD);
      motorFR.run(FORWARD);
      motorBL.run(BACKWARD);
      motorBR.run(BACKWARD); 
    break;
    
    case LEFT:
      motorFL.run(BACKWARD);
      motorFR.run(FORWARD);
      motorBL.run(FORWARD);
      motorBR.run(BACKWARD); 
    break;
    
    case RIGHT:
      motorFL.run(FORWARD);
      motorFR.run(BACKWARD);
      motorBL.run(BACKWARD);
      motorBR.run(FORWARD); 
    break;
    
    case STOP:
    default:
      motorFL.run(STOP);
      motorFR.run(STOP);
      motorBL.run(STOP);
      motorBR.run(STOP); 
  }
}

void forward(unsigned long time)
{
  time = (time == 0) ? currMoveTime : time;
  targetTime = millis() + time;
  dir = (TDirection) FWD;
  move(currMoveSpeed);
}

void backward(unsigned long time)
{  
  time = (time == 0) ? currMoveTime : time;
  targetTime = millis() + time;
  dir = (TDirection) BWD;
  move(currMoveSpeed);
}

void left(unsigned long time)
{
  time = (time == 0) ? currTurnTime : time;
  targetTime = millis() + time;
  dir = (TDirection) LEFT;
  move(currTurnSpeed);
}

void right(unsigned long time)
{
  time = (time == 0) ? currTurnTime : time;
  targetTime = millis() + time;
  dir = (TDirection) RIGHT;
  move(currTurnSpeed);
}

void stop()
{
  dir = (TDirection) STOP;
  move(0);
}
