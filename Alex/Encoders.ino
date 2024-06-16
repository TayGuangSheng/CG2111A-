//ENCODERS - Setup Register + Left, Right ISRs

// Encoder Tick for moving straight
volatile unsigned long leftForwardTicks; 
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks; 
volatile unsigned long rightReverseTicks;

// Encoder Ticks for turning
volatile unsigned long leftForwardTicksTurns; 
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns; 
volatile unsigned long rightReverseTicksTurns;

// Revolutions of wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

void enablePullups() 
{
  //Sets pins 18,19 to pullup resistor mode
  int state = 0b00001100;
  DDRD  &= ~state; //set INPUT
  PORTD |= state; //set HIGH
}

void setupEINT() 
{
  //Enable INT2, INT3 interrupts
  EIMSK |= (1 << INT3) | (1 << INT2);
  //Set interrupt trigger mode to rising edge
  EICRA |= (1 << ISC31) | (1 << ISC30) | (1 << ISC21) | (1 << ISC20);
}

ISR(INT2_vect)  //Left ISR
{ 
  switch(dir)
  {
    case FWD:
      leftForwardTicks++;
      forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
    break;

    case BWD:
      leftReverseTicks++;
      reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
    break;

    case LEFT:
      leftReverseTicks++;
    break;

    case RIGHT:
      leftForwardTicks++;
    break;
  }
}

ISR(INT3_vect) //Right ISR
{ 
  switch(dir)
  {
    case FWD:
      rightForwardTicks++;
    break;

    case BWD:
      rightReverseTicks++;
    break;

    case LEFT:
      rightForwardTicks++;
    break;

    case RIGHT:
      rightReverseTicks++;
    break;
  }
}
