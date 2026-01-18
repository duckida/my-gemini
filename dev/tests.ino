void testSensorPD(int target) {
  float pid = PID(KP, KD, leftSensorValue, target);
  setMotors(60, 60 + pid); 
}

void testEncoderPD(int target) {
  float pid = PID(KP, KD, encoderDifference, target);
  setMotors(60, 60 + pid); 
}

void testDriveDistance(int distance) {
  driveDistance(60, 60, distance);
  delay(3000);
}

void testEncoders() {
  printEncoders();
  delay(200);
}
