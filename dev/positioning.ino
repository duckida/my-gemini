// Gemini positioning module
// `normalizeDirection()` allows the direction to be normalized
// `updateDirection()` allows the direction to be normalized & set as `robotDir`
// `updatePosition()` moves the robot forward 1 square, respecting direction
// `updatePositionBack()` does the same but moves it back 1 square
// `printPosition` prints the x, y, and direction to Serial1

// normalizes turn direction to 0-360º
int normalizeDirection(int turnDir) {
  int newDirection = robotDir + turnDir;

  newDirection = newDirection % 360;

  if (newDirection < 0) {
    newDirection = 360 + newDirection;
  }
  
  return newDirection;
}

// sets the normalized direction
void updateDirection(int turnDir) {
   robotDir = normalizeDirection(turnDir);
}

// Move forward by 1, respecting direction
void updatePosition() {
  switch(robotDir) {
    case 0:
      robotY = robotY + 1;
      break;
    case 90:
      robotX = robotX + 1;
      break;
    case 180:
      robotY = robotY - 1;
      break;
    case 270:
      robotX = robotX - 1;
      break;
  }
}

void updatePositionBack() {
  switch(robotDir) {
    case 0:
      robotY = robotY - 1;
      break;
    case 90:
      robotX = robotX - 1;
      break;
    case 180:
      robotY = robotY + 1;
      break;
    case 270:
      robotX = robotX + 1;
      break;
  }
}

void printPosition() {
  Serial1.print("X: ");
  Serial1.print(robotX);
  Serial1.print(" Y: ");
  Serial1.print(robotY);
  Serial1.print(" D: ");
  Serial1.print(robotDir);
  Serial1.println();
}
