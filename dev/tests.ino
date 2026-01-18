void testSensorPD(int target) {
  float adj = sensorPid.calculate(leftSensorValue, target)
  setMotors(60, 60 + adj); 
}

void testEncoderPD(int target) {
  float adj = motorPid.calculate(encoderDifference, 0);
  setMotors(60, 60 + adj); 
}

void testDriveDistance(int distance) {
  driveDistance(60, 60, distance);
  delay(5000);
}

void testDriveAngle(int angle) {
  driveAngle(60, 60, angle);
  delay(5000);
}

void testEncoders() {
  printEncoders();
  delay(200);
}
