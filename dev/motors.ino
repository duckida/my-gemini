// Gemini Motors Abstractor
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
