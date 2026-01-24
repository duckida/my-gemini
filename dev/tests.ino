void testSensorPD(int target) {
  float adj = sensorPid.calculate(leftSensorValue, target);
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
  driveAngle(60, angle);
  delay(5000);
}

void testEncoders() {
  printEncoders();
  delay(200);
}

// Reused from UKMARSBOT
void testWallFollow() {
  if (leftSensorValue <= 10) {
    SerialBT.println("I see a gap...");
    delay(500);
    driveDistance(60, 60, 145); //135
    SerialBT.println("Time to turn!");
    stop();
    delay(500);
    driveAngle(60, 90);
    SerialBT.println("On we go!");
    driveDistance(60, 60, 25);
  }
  if (frontSensorValue > 80) {
    stop();
    driveAngle(60, -90);
    delay(200);
    stop();
  }

  float adj = sensorPid.calculate(leftSensorValue, 25);
  setMotors(60 - adj, 60 + adj);
}

void testWallFollowNext() {
  if (leftSensorValue <= LEFT_GAP) {
    driveDistance(60, 60, 180);
    delay(300);
    driveAngle(60, 90);
    delay(300);
  }

  else if (frontSensorValue >= FRONT_WALL) {
    driveAngle(60, -90);
  }

  else {
    float adj = sensorPid.calculate(leftSensorValue, 25);
    setMotors(60 - adj, 60 + adj);
  }

}

void testWalls() {
  if (leftSensorValue > LEFT_GAP) {
    on(20);
  } else {
    off(20);
  }

  if (frontSensorValue > FRONT_WALL) {
    on(19);
  } else {
    off(19);
  }

  if (rightSensorValue > RIGHT_GAP) {
    on(19);
  } else {
    off(19);
  }
  
}
