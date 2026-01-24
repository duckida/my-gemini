// Gemini Motors
// Step 1. `initMotors();` which sets up the pins and enables the driver
// The, use `setMotors(left, right);` with speeds up to 256
// Use `stop();` to stop

// Motor Pins
#define M_RIGHT_FORWARD   5
#define M_RIGHT_BACKWARD   4

#define M_LEFT_FORWARD   2
#define M_LEFT_BACKWARD   3

#define SLP    7

void initMotors() {
  pinMode(M_LEFT_FORWARD, OUTPUT);
  pinMode(M_LEFT_BACKWARD, OUTPUT);
  pinMode(M_RIGHT_FORWARD, OUTPUT);
  pinMode(M_RIGHT_BACKWARD, OUTPUT);

  pinMode(SLP, OUTPUT);

  // enable DRV8833
  digitalWrite(SLP, HIGH);
}

void setMotors(int left, int right) {
  int MAX_SPEED = 256;

  int absLeft = abs(left); // No matter if positive or negative, the correct 256 - speed can be done
  int absRight = abs(right);
  
  // IF positive number, Set the forward speeds to max, and the reverse speed to max - target
  // IF negative number, Set the backward speeds to max, and the forward speed to max - target

  if (left >= 0) {
    analogWrite(M_LEFT_FORWARD, MAX_SPEED);
    analogWrite(M_LEFT_BACKWARD, MAX_SPEED - absLeft);
  } 
  else {
    analogWrite(M_LEFT_FORWARD, MAX_SPEED - absLeft);
    analogWrite(M_LEFT_BACKWARD, MAX_SPEED);
  }

  if (right >= 0) {
    analogWrite(M_RIGHT_FORWARD, MAX_SPEED);
    analogWrite(M_RIGHT_BACKWARD, MAX_SPEED - absRight);
  } 
  else {
    analogWrite(M_RIGHT_FORWARD, MAX_SPEED - absRight);
    analogWrite(M_RIGHT_BACKWARD, MAX_SPEED);
  }

  
}

void stop() {
  analogWrite(M_LEFT_FORWARD, 0);
  analogWrite(M_LEFT_BACKWARD, 0);
  
  analogWrite(M_RIGHT_FORWARD, 0);
  analogWrite(M_RIGHT_BACKWARD, 0);
}

void driveDistance(int lSpeed, int rSpeed, float mm) {
  leftEncoderValue = 0;
  rightEncoderValue = 0;
  
  int pulses = abs(calculateDistancePulses(mm));

  int encoderAvg = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2;

  int direction = (mm >= 0) ? 1 : -1; // 1 for forward, -1 for back
  int moveL = lSpeed * direction;
  int moveR = rSpeed * direction;
  
  setMotors(moveL, moveR);

  while (encoderAvg < pulses) {
    float adjustment = motorPid.calculate(encoderDifference, 0);
    setMotors(moveL, moveR + adjustment); 
    encoderAvg = encoderAvg = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2;
  }

  
  stop();
}



// NOTE: This function is written by AI
void driveAngle(int speed, float deg) {
  // Use same speed magnitude for both wheels, opposite directions
  leftEncoderValue = 0;
  rightEncoderValue = 0;

  float targetPulses = calculateAnglePulses(abs(deg)); // Always positive
  bool turnRight = (deg < 0);

  // Determine motor directions
  int leftMotor, rightMotor;
  if (turnRight) {
    leftMotor = speed;   // Left forward
    rightMotor = -speed; // Right backward
  } else {
    leftMotor = -speed;  // Left backward
    rightMotor = speed;  // Right forward
  }

  setMotors(leftMotor, rightMotor);

  // For right turn: encoderDifference = right - left becomes MORE NEGATIVE
  // For left turn: encoderDifference becomes MORE POSITIVE
  // So we track total rotation via sum of absolute changes or use a signed target

  // Better: use total angular displacement = (left + right)/2 in terms of rotation?
  // Simpler: use |left| + |right| since both wheels move same distance in opposite directions

  long totalMoved = 0;
  while (totalMoved < targetPulses) {
    totalMoved = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2;
    delay(2); // small delay to avoid busy-wait overload
  }

  stop();
}
