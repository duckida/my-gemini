void testSensorPD(int target) {
  float adj = sensorPid.calculate(leftSensorValue, target);
  setMotors(60, 60 + adj);
}

void testEncoderPD(int target) {
  float adj = motorPid.calculate(encoderDifference, 0);
  setMotors(60, 60 + adj);
}

void testDriveDistance(int distance) {
  motion.driveDistance(distance);
  delay(5000);
}

void testDriveAngle(int angle) {
  motion.driveAngle(angle);
  while(!motion.completed()) {};
}

void testEncoders() {
  printEncoders();
  delay(200);
}
