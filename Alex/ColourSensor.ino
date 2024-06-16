#define S0 29
#define S1 31
#define S2 33
#define S3 35
#define sensorOut 37

// Frequency read by photodiodes
unsigned long redFreq;
unsigned long greenFreq;
unsigned long blueFreq;

// Setting up colour pins
void setupColour() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
}

// Getting average frequency
int getAvgFreq() {
  int reading;
  int total = 0;
  for (int i = 0; i < 5; i++) {
    reading = pulseIn(sensorOut, LOW);
    total += reading;
    delay(20);
  }
  return total / 5;
}

void getColour(){
  // Setting Red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(100);

  // Reading the output frequency for RED
  redFreq = getAvgFreq();
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for GREEN
  greenFreq = getAvgFreq();
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for BLUE
  blueFreq = getAvgFreq();
  delay(100);
}

