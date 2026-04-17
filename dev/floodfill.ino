// Gemini Floodfill module
#include <queue>

// --- DEFINITIONS ---

// distances (cell percentage)
#define BACK_TO_MIDDLE 27
#define MIDDLE_TO_SP1 26 //16
#define SP1_TO_SP2 5 // 10
#define SP2_TO_SP3 5 
#define SP3_TO_ALMOST_MIDDLE 41 // 84-15=69 == 59 is the one 42
#define ALMOST_TO_MIDDLE 18 //15 17
// these sum to 95%
// the extra 5% is if there's no wall in front
#define EXTRA_5_PERCENT 5 // 5

// Queue setup
struct Point {
  uint8_t x;
  uint8_t y;
};

std::queue<Point> q;

// Cell structure
struct Cell {
  uint8_t x;
  uint8_t y;
  uint8_t cost;

  bool walls[4];
};


// defining the maze
// 16x16 also works!
const uint8_t MAZE_WIDTH = 3;
const uint8_t MAZE_HEIGHT = 6;

const uint8_t GOAL_X = 2;
const uint8_t GOAL_Y = 5;

uint8_t targetX = GOAL_X;
uint8_t targetY = GOAL_Y;

Cell maze[MAZE_WIDTH][MAZE_HEIGHT]; // in order x, y

// maze state
#define DISCOVERING 0
#define RETURNING 1
#define FAST 2
int mazeState = 0;

// --- FUNCTIONS ---

// converts 0-360 direction to wall directions
// 0→0, 90→1, 180→2, 270→3
uint8_t convertDirection(int compassDirection) {
  if (compassDirection == 0) {return 0;}
  else if (compassDirection == 90) {return 1;}
  else if (compassDirection == 180) {return 2;}
  else if (compassDirection == 270) {return 3;}
  else {return 0;}
}

// check if a cell exists in the maze
bool cellExists(int x, int y) {
  if ((x >= MAZE_WIDTH) || (x < 0) || (y >= MAZE_HEIGHT) || (y < 0)) { return false; }
  else { return true; }
}

// sets all costs to 255 and sets the goal to 0
void resetMaze() {
  for(uint8_t i=0; i<MAZE_WIDTH; i++) {
    for(uint8_t j=0; j<MAZE_HEIGHT; j++) {
      maze[i][j].cost = 255;
    }
  }
  maze[targetX][targetY].cost = 0; // set the goal cost to 0
}

// checks if a wall is present at x, y, wallDirection (0-3)
bool checkWall(uint8_t x, uint8_t y, int wallDirection) {
  return maze[x][y].walls[wallDirection];
} // returns True is wall present

// sets walls on both cells
void setWall(bool state, uint8_t x, uint8_t y, int direction) {
  uint8_t wallDirection = convertDirection(direction);
  
  maze[x][y].walls[wallDirection] = state;

  uint8_t neighborWallX = x;
  uint8_t neighborWallY = y;
  uint8_t neighborWallDir;
  
  if (wallDirection == 0) {neighborWallY = y+1; neighborWallDir = 2;}
  else if (wallDirection == 1) {neighborWallX = x+1; neighborWallDir = 3;}
  else if (wallDirection == 2) {neighborWallY = y-1; neighborWallDir = 0;}
  else if (wallDirection == 3) {neighborWallX = x-1; neighborWallDir = 1;}

  if (cellExists(neighborWallX, neighborWallY)) {
    maze[neighborWallX][neighborWallY].walls[neighborWallDir] = state;
  }
}

void updateFlood() {
  resetMaze();
  
  while(!q.empty()) q.pop(); // clear the queue

  q.push({targetX, targetY}); // add the goal to the back of the queue
  
  while (!q.empty()) { 
    Point currentExaminingCell = q.front();
    q.pop(); // remove front element
    
    uint8_t x = currentExaminingCell.x;
    uint8_t y = currentExaminingCell.y;

    
    for (uint8_t wallDirection = 0; wallDirection < 4; wallDirection++) { // for each direction
      if (checkWall(x, y, wallDirection) == false) { // there is no wall
        
        // get neighbors of the current examining cell
        uint8_t neighborWallX = x;
        uint8_t neighborWallY = y;
        
        if (wallDirection == 0) {neighborWallY = y+1;}
        else if (wallDirection == 1) {neighborWallX = x+1;}
        else if (wallDirection == 2) {neighborWallY = y-1;}
        else if (wallDirection == 3) {neighborWallX = x-1;}

        if (cellExists(neighborWallX, neighborWallY) && maze[neighborWallX][neighborWallY].cost == 255) { // it has not already been costed
          maze[neighborWallX][neighborWallY].cost = maze[x][y].cost + 1; // set it's cost to the currentCell + 1
          q.push({neighborWallX, neighborWallY});
        }
      }
    }
  }
}


// WALL SENSOR FUNCTIONS

// for multiple checks
// +1 for yes, -1 for no
// then we check: >0? then wall
int leftWall = 0; 
int rightWall = 0;

void checkSideWalls() {
  printSensors();
  
  if (leftSensorValue > LEFT_GAP) { // there's a wall on the left 
    leftWall = leftWall + 1;
  } else {
    leftWall = leftWall - 1;
  }

  if (rightSensorValue > RIGHT_GAP) { // there's a wall on the right 
    rightWall = rightWall + 1;
  } else {
    rightWall = rightWall - 1;
  }
}

void updateSideWalls() {
  if (leftWall > 0) { // we're sure there's a wall
    setWall(true, robotX, robotY, normalizeDirection(-90)); 
  } 
  else {
    setWall(false, robotX, robotY, normalizeDirection(-90)); 
  }

  if (rightWall > 0) { // we're sure there's a wall
    setWall(true, robotX, robotY, normalizeDirection(90)); 
  } 
  else {
    setWall(false, robotX, robotY, normalizeDirection(90)); 
  }
}

void checkAndUpdateFrontWall() {
  if (frontSensorValue >= FRONT_WALL) { // wall in front
    Serial1.println("WALL ON FRONT");
    setWall(true, robotX, robotY, robotDir); 
  }
  sendSensorReadings(0); // sends to mousefriend app
}

// --- MAZE SOLVING LOOP ---
// to return it just sets target to 0,0

void mazeLoop() {
  /*
[x] Check walls 
[x] Set walls based on checks
[x] Reflood
[x] LowestCostCell = 0; // north

[x] For cell in neighbors
[x] If cell.cost < lowestcostcell:
[x]    Lowestcostcell = cell

[x] Turn to the direction of LowestCostCell
[x] Drive forward
[x] Update position 

[] From middle, read front wall and reflood
[] Decide which cell to go (flood which needs wall readings)
[] Go to sensing point
[] 

   */

  leftWall = 0; // reset wall counters
  rightWall = 0;


  if (robotX == targetX && robotY == targetY) {
    stop();
    delay(5000);
    
    switch(mazeState) {
      case DISCOVERING: // we've just finished discovering
        targetX = 0;
        targetY = 0;
        mazeState = RETURNING;
        break; 
      case RETURNING: // we've just returned to (0,0)
        targetX = GOAL_X;
        targetY = GOAL_Y;
        mazeState = DISCOVERING;
        //updateFlood();
        //mazeState = FAST; // time to go FAST!
        break;
    }

    return;
  }

  checkAndUpdateFrontWall(); 
  
  updateFlood();
  sendDebugState();

  Cell lowestCostCell = maze[robotX][robotY]; // init with the current cell's cost
  int lowestCostDirection = 0; // the cell with lowest cost's absolute direction

  // check what the cheapest cell to go to is
  for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) { // for each direction
    uint8_t neighborX = robotX;
    uint8_t neighborY = robotY;
    int neighborDir;
    
    if (neighborIndex == 0) {neighborY = robotY+1; neighborDir = 0;}
    else if (neighborIndex == 1) {neighborX = robotX+1; neighborDir = 90;}
    else if (neighborIndex == 2) {neighborY = robotY-1; neighborDir = 180;}
    else if (neighborIndex == 3) {neighborX = robotX-1; neighborDir = 270;}

    if (cellExists(neighborX, neighborY)) {
      Cell neighbor = maze[neighborX][neighborY];
  
      if (neighbor.cost < lowestCostCell.cost && !checkWall(robotX, robotY, convertDirection(neighborDir)))  { // the neighbor is the cheapest seen so far & there's no blocking wall
        lowestCostCell = neighbor;
        lowestCostDirection = neighborDir;
      }
    }
  }

  if (lowestCostCell.cost == maze[robotX][robotY].cost) { // same cost as current
    stop(); // no valid move found
    return;
  }

  // turn until facing the direction of the lowest cost cell (this part is written by AI)
  int relative = (lowestCostDirection - robotDir + 360) % 360;
  if (relative > 180) relative -= 360;

  if (relative == 180) { // turn in 90º intervals
    delay(100);
    
    // turn the first 90º
    motion.driveAngle(90);
    while (!motion.completed()) {}
    delay(100);  

    // turn the next 90º
    motion.driveAngle(90);
    while (!motion.completed()) {}
    delay(100);  

    if (checkWall(robotX, robotY, convertDirection(normalizeDirection(180)))) { // make sure there's a wall behind
      // ram into the wall
      setMotors(-100, -100);
      delay(600);
      stop();

      delay(100);
      globalHeading = round(globalHeading / 90.0) * 90.0; // reset the heading to a nice flat one
      delay(50);
  
      // come back to the center
      motion.driveCell(BACK_TO_MIDDLE, DRIVE_PID_NONE);
      while (!motion.completed()) {}
      delay(300); 
    }
    
  } else if (relative != 0) {
    delay(100);
    
    motion.driveAngle(relative);
    while (!motion.completed()) {}
    delay(100);
  } 

  // drive to the next cell's sensing point
  //motion.driveDistance(30);
  motion.driveCell(MIDDLE_TO_SP1, DRIVE_PID_NONE);
  while (!motion.completed()) {}
  
  updatePosition();
  
  on(13);
  checkSideWalls(); // check the walls once at sensepoint 1
  sendSensorReadings(0); // send to mocusefriend
  off(13);
  

  motion.driveCell(SP1_TO_SP2, DRIVE_PID_NONE); // drive the first segment without PID
  while (!motion.completed()) {}
  
  checkSideWalls(); // check the walls again (sensepoint 2)
  updateSideWalls(); // decide if there's a wall or not
  sendSensorReadings(1); // send to mousefriend

  motion.driveCell(SP2_TO_SP3, DRIVE_PID_NONE); // drive the next segment without PID
  while (!motion.completed()) {}
  
  checkSideWalls(); // check the walls again (sensepoint 3)
  updateSideWalls(); // decide if there's a wall or not
  sendSensorReadings(2); // send to mousefriend

  if (checkWall(robotX, robotY, convertDirection(normalizeDirection(270)))) { // left wall present
    motion.driveCell(SP3_TO_ALMOST_MIDDLE, DRIVE_PID_LEFT); // drive a litle more of the way with PID
    while (!motion.completed()) {}
  }
  else if (checkWall(robotX, robotY, convertDirection(normalizeDirection(90)))) { // right wall present
    motion.driveCell(SP3_TO_ALMOST_MIDDLE, DRIVE_PID_RIGHT); // drive a litle more of the way with PID 40
     while (!motion.completed()) {}
  }
  else {
    motion.driveCell(SP3_TO_ALMOST_MIDDLE, DRIVE_PID_NONE); // 40
    while (!motion.completed()) {}
  }
 
  //checkSideWalls(); // check the walls again (sensepoint 3)
  //sendSensorReadings(2); // send to mousefriend
  
  //updateSideWalls(); // final descision

  motion.driveCell(ALMOST_TO_MIDDLE + EXTRA_5_PERCENT, DRIVE_PID_NONE, true); // use braking
  while (!motion.completed()) {}



  /*if (frontSensorValue >= FRONT_WALL) { // wall in front
    motion.driveCell(ALMOST_TO_MIDDLE, DRIVE_PID_NONE, true); // use braking & go the extra 5%
    while (!motion.completed()) {}
  }
  else {
      motion.driveCell(ALMOST_TO_MIDDLE + EXTRA_5_PERCENT, DRIVE_PID_NONE, true); // use braking
      while (!motion.completed()) {}
  }*/
 
  sendDebugState();

  //delay(500);
  //delay(1000);
}
