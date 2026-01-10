// Emitters
#define E_LEFT_RIGHT   22
#define E_FRONT   21

#define S_LEFT 28
#define S_FRONT 27
#define S_RIGHT 26

void initSensors() {
  pinMode(E_LEFT_RIGHT, OUTPUT);
  pinMode(E_FRONT, OUTPUT);
  
  pinMode(S_LEFT, INPUT);
  pinMode(S_FRONT, INPUT);
  pinMode(S_RIGHT, INPUT);

  attachInterrupt(S_LEFT, updateSensors, CHANGE);
}

void updateSensors() {
  // read the sensors unlit
  int l_unlit = analogRead(S_LEFT);
  int f_unlit = analogRead(S_FRONT);
  int r_unlit = analogRead(S_RIGHT);

  // turn on the emitters
  digitalWrite(E_LEFT_RIGHT, HIGH);
  digitalWrite(E_FRONT, HIGH);

  // wait for sensors to stabilize
  delayMicroseconds(50);

  // read the sensors and subtract the unlit value
  leftSensorValue = analogRead(S_LEFT) - l_unlit;
  frontSensorValue = analogRead(S_FRONT) - f_unlit;
  rightSensorValue = analogRead(S_RIGHT) - r_unlit;

  digitalWrite(E_LEFT_RIGHT, LOW);
  digitalWrite(E_FRONT, LOW);
}

void printSensors() {
  SerialBT.print(leftSensorValue);
  SerialBT.print(" ");
  SerialBT.print(frontSensorValue);
  SerialBT.print(" ");
  SerialBT.print(rightSensorValue);
  SerialBT.println();
}
