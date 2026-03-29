// Gemini Floodfill module

#include <queue>

// --- DEFINITIONS ---

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
const int MAZE_WIDTH = 3;
const int MAZE_HEIGHT = 3;

const uint8_t targetX = 2;
const uint8_t targetY = 2;

Cell maze[MAZE_WIDTH][MAZE_HEIGHT]; // in order x, y

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
  for(int i=0; i<MAZE_WIDTH; i++) {
    for(int j=0; j<MAZE_HEIGHT; j++) {
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
void setWall(uint8_t x, uint8_t y, int direction) {
  uint8_t wallDirection = convertDirection(direction);
  
  maze[x][y].walls[wallDirection] = true;

  uint8_t neighborWallX = x;
  uint8_t neighborWallY = y;
  uint8_t neighborWallDir;
  
  if (wallDirection == 0) {neighborWallY = y+1; neighborWallDir = 2;}
  else if (wallDirection == 1) {neighborWallX = x+1; neighborWallDir = 3;}
  else if (wallDirection == 2) {neighborWallY = y-1; neighborWallDir = 0;}
  else if (wallDirection == 3) {neighborWallX = x-1; neighborWallDir = 1;}

  if (cellExists(neighborWallX, neighborWallY)) {
    maze[neighborWallX][neighborWallY].walls[neighborWallDir] = true;
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

    
    for (int wallDirection = 0; wallDirection < 4; wallDirection++) { // for each direction
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

// --- MAZE SOLVING LOOP ---
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


   */

  if (robotX == targetX && robotY == targetY) {
    stop();
    return;
  }

  if (leftSensorValue > LEFT_GAP) { // there's a wall on the left 
    setWall(robotX, robotY, normalizeDirection(-90)); 
  }

  if (rightSensorValue > RIGHT_GAP) { // there's a wall on the right 
    setWall(robotX, robotY, normalizeDirection(90)); 
  }

  if (frontSensorValue >= FRONT_WALL) { // wall in front
    setWall(robotX, robotY, normalizeDirection(0)); 
  }

  updateFlood();

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
  motion.driveAngle(relative);
  while (!motion.completed()) {}

  // drive to the next cell
  motion.driveDistance(180);
  while (!motion.completed()) {}
  updatePosition();
  
  sendMazeState();
}
