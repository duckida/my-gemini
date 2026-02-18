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

int f_unlit = 0;
int r_unlit = 0;
int l_unlit = 0;

void updateSensors() {
  // --- 1. READ SIDE SENSORS ---
  l_unlit = analogRead(S_LEFT);
  r_unlit = analogRead(S_RIGHT);
  
  digitalWrite(E_LEFT_RIGHT, HIGH);
  delayMicroseconds(75); 

  // Calculate difference and ensure it's not negative
  leftSensorValue = max(0, (int)analogRead(S_LEFT) - l_unlit);
  rightSensorValue = max(0, (int)analogRead(S_RIGHT) - r_unlit);

  digitalWrite(E_LEFT_RIGHT, LOW);

  delayMicroseconds(75); 

  // --- 2. READ FRONT SENSOR ---
  f_unlit = analogRead(S_FRONT);

  digitalWrite(E_FRONT, HIGH);
  delayMicroseconds(75);
  
  frontSensorValue = max(0, (int)analogRead(S_FRONT) - f_unlit);
  
  digitalWrite(E_FRONT, LOW);


  
  // Update the rolling average lists
  //updateLeftSensorList(leftSensorValue);
  //updateFrontSensorList(frontSensorValue);
  //updateRightSensorList(rightSensorValue);

  //int leftSum = 0;
  //int frontSum = 0;
  //int rightSum = 0;

  //for (int i = 0; i < 10; i++) {
  //  leftSum = leftSum + leftSensor[i];
  //  frontSum = frontSum + frontSensor[i];
  //  rightSum = rightSum + rightSensor[i];
  //}

  //leftSensorValue = leftSum / 10;
  //frontSensorValue = frontSum  / 10;
  //rightSensorValue = rightSum / 10;
}

void printSensors() {
  Serial1.print(leftSensorValue);
  Serial1.print(" ");
  Serial1.print(frontSensorValue);
  Serial1.print(" ");
  Serial1.print(rightSensorValue);
  Serial1.println();
}

void updateLeftSensorList(int value) {
  for (int i = 0; i < 9; i++) {
    leftSensor[i] = leftSensor[i+1];
  }
  leftSensor[9] = value; // New value at the end
}

void updateFrontSensorList(int value) {
  for (int i = 0; i < 9; i++) {
      frontSensor[i] = frontSensor[i+1];
  }

  frontSensor[9] = value;
}


void updateRightSensorList(int value) {
  for (int i = 0; i < 9; i++) {
      rightSensor[i] = rightSensor[i+1];
  }

  rightSensor[9] = value;
}
