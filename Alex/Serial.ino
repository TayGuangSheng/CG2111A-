void setupSerial() 
{
  Serial.begin(9600);
}

void startSerial() {
}

int readSerial(char *buffer)
{
  int count = 0;
  while (Serial.available()) {buffer[count++] = Serial.read();}
  return count;
}

void writeSerial(const char *buffer, int len)
{
  Serial.write(buffer, len);
}

//TODO: Convert to Machine Language. Boilerplate Code does not work.