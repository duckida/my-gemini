#include "pid.h"
#include "motion.h"
#include "RPi_Pico_TimerInterrupt.h"

// --- GLOBAL VARIABLES ---

// Sensor values
volatile int leftSensorValue = 0;
volatile int frontSensorValue = 0;
volatile int rightSensorValue = 0;

//int leftSensor[10];
//int frontSensor[10];
//int rightSensor[10];

// Wall thresholds
const int LEFT_GAP = 10;
const int FRONT_WALL = 22; //30
const int RIGHT_GAP = 10;

// Encoder values
volatile int leftEncoderValue = 0;
volatile int rightEncoderValue = 0;
volatile int encoderDifference = 0; // right - left

// MPU values
volatile float heading = 0.0; // for turns
volatile float globalHeading = 0.0; // for debugging

// PD controller values
const float SENSOR_KP = 1.0; // 2.0
const float SENSOR_KD = 0.8; // 0.5
int LEFT_TARGET = 28; //24
int RIGHT_TARGET = 32; //29

// Debug levels
#define DEBUG_NONE 0
#define DEBUG_MINIMAL 1
#define DEBUG_FULL 2

PID sensorPid(SENSOR_KP, SENSOR_KD);

// Timer setup
RPI_PICO_Timer ITimer(0);

// Motion controller
Motion motion; 
// note: 0.9 and 0.5 are good Kp/Kd value for motion


// Positioning variables
int robotX = 0;
int robotY = 0;
int robotDir = 0;


int mode; // 0 unconfigured 1 print sensors 2 test distance 3 test angle 4 wall follow 5 hardcoded 6 test MPU 7 solve maze

// --- SETUP ---
void setup() {

  // Setup BLE Serial
  Serial1.begin(57600);

  // Initialize components
  initMotors();
  initButtons();
  initLeds();
  initEncoders();
  //initMpu();
  
  delay(2000);

  // start timer
  float frequency = 500.0f;
  ITimer.attachInterrupt(frequency, TimerHandler);

  // get config details
  Serial1.println("------GEMINI 2026.3.1------");
  Serial1.println();

  //int Kp = askForData("KP").toFloat();
  //int Kd = askForData("KD").toFloat();
  
  motion.setup(0.9, 0.5); // 0.9, 0.5

  Serial1.println("1: print sensors, 2: test distance, 3: test angle, 4: wall follow, 5: hardcoded, 6: test MPU, 7: solve maze, 8: print encoders 9. drive cell 10. drive cell advanced");
  mode = askForData("Select mode:").toInt();
  
  if (mode == 7) { // if solving
    globalHeading = 0.0;
    setDebugLevel(DEBUG_MINIMAL); 
    sendDebugState();
    setWall(true,0,0,90); // set the left and write walls as present
    setWall(true,0,0,270);

    LEFT_TARGET = leftSensorValue;
    RIGHT_TARGET = rightSensorValue;
    
    //motion.driveDistance(52); // start flush to the wall, go to the center
    motion.driveCell(27, DRIVE_PID_NONE);
    while (!motion.completed()) {}
  }
}

// -- TIMER RUNNER --
bool TimerHandler(struct repeating_timer *t)
{
  motion.update();
  return true; // Return true to keep the timer repeating
}

void loop() {
  //updateMpu();
  switch(mode) {
    case 1:
      printSensors();
      delay(200);
      break;
    case 2:
      testDriveDistance(1000);
      Serial1.println(robotY);
      break;
    case 3:
      testDriveAngle(-90);
      // printMpu();
      break;
    case 4:
      wallFollow();
      printPosition();
      break;
    case 5:
      testHardCodedMaze();
      break;
    case 6:
      printMpu();
      delay(200);
      break;
    case 7:
      mazeLoop();
      // printPosition();
      break;
    case 8:
      printEncoders();
      delay(100);
      break;
    case 9:
      testDriveCell(100);
      break;
    case 10:
      int distance = askForData("Distance:").toInt();
      testDriveCell(distance);
      printEncoders();
      break;
  }
};

// -- SECOND CORE (USED FOR SENSORS & MPU) --
void setup1() {
  initSensors();
  initMpu();
  resetHeading();
}

void loop1() {
  updateSensors();
  updateMpu();
};
