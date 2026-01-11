// Gemini Sensors
// Step 1. `initSensors();` which sets up the pins
// The, use `updateSensors();` in core 2 or in an interrupt
// Use the values from `leftSensorValue`, `frontSensorValue`, and `rightSensorValue`
// Use `printSensors();` to output the values in format L F R

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
}

void updateSensors() {
  // --- 1. READ SIDE SENSORS ---
  int l_unlit = analogRead(S_LEFT);
  int r_unlit = analogRead(S_RIGHT);

  digitalWrite(E_LEFT_RIGHT, HIGH);
  delayMicroseconds(75); 

  // Calculate difference and ensure it's not negative
  leftSensorValue = max(0, (int)analogRead(S_LEFT) - l_unlit);
  rightSensorValue = max(0, (int)analogRead(S_RIGHT) - r_unlit);

  digitalWrite(E_LEFT_RIGHT, LOW);

  // --- 2. READ FRONT SENSOR ---
  int f_unlit = analogRead(S_FRONT);

  digitalWrite(E_FRONT, HIGH);
  delayMicroseconds(75);
  
  frontSensorValue = max(0, (int)analogRead(S_FRONT) - f_unlit);
  
  digitalWrite(E_FRONT, LOW);
  
  delayMicroseconds(75);
}

void printSensors() {
  SerialBT.print(leftSensorValue);
  SerialBT.print(" ");
  SerialBT.print(frontSensorValue);
  SerialBT.print(" ");
  SerialBT.print(rightSensorValue);
  SerialBT.println();
}
