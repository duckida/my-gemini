void testSensorPD(int target) {
  float adj = sensorPid.calculate(leftSensorValue, target);
  setMotors(60, 60 + adj);
}


void testDriveDistance(int distance) {
  motion.driveDistance(distance);
  while(!motion.completed()) {};
  delay(2000);
}


void testDriveCell(int cellPercentage) {
  motion.driveCell(cellPercentage);
  while(!motion.completed()) {};
  delay(2000);
}

void testDriveAngle(int angle) {
  motion.driveAngle(angle);
  while(!motion.completed()) {};
  delay(2000);
}

void testEncoders() {
  printEncoders();
  delay(200);
}

void testHardCodedMaze() {
  testDriveDistance(180);
  testDriveDistance(180);

  testDriveAngle(90);
  testDriveDistance(180);
  testDriveAngle(90);
  
  testDriveDistance(180);
  testDriveAngle(-90);
  testDriveDistance(180);

  testDriveAngle(-90);
  testDriveDistance(180);
  testDriveAngle(90);
  testDriveAngle(90);
  
  testDriveDistance(180);
  testDriveDistance(180);
}
