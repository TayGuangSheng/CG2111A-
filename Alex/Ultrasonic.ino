#define trigPin 23
#define echoPin 22
#define SPEED_OF_SOUND 0.0345

void setupSensor(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

uint32_t getSensorDistance(){
  // Clears trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets trigPin to HIGH for 10 microsecs
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long microsecs = pulseIn(echoPin, HIGH);
  return microsecs * SPEED_OF_SOUND / 2;
}

