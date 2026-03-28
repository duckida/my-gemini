#include "pid.h"
#include "motion.h"
#include "RPi_Pico_TimerInterrupt.h"

// --- GLOBAL VARIABLES ---

// Sensor values
volatile int leftSensorValue = 0;
volatile int frontSensorValue = 0;
volatile int rightSensorValue = 0;

int leftSensor[10];
int frontSensor[10];
int rightSensor[10];

// Wall thresholds
const int LEFT_GAP = 7;
const int FRONT_WALL = 40;
const int RIGHT_GAP = 6;

// Encoder values
volatile int leftEncoderValue = 0;
volatile int rightEncoderValue = 0;
volatile int encoderDifference = 0; // right - left

// MPU values
volatile float heading = 0.0;

// PD controller values
const float SENSOR_KP = 1.0;
const float SENSOR_KD = 0.5;
const int TARGET = 25;

PID sensorPid(SENSOR_KP, SENSOR_KD);

// Timer setup
RPI_PICO_Timer ITimer(0);

// Motion controller
Motion motion; 
// note: 0.9 and 0.5 are good Kp/Kd value for motion

int mode; // 0 unconfigured 1 print sensors 2 test distance 3 test angle 4 wall follow 5 hardcoded

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
  Serial1.println("------GEMINI------");
  Serial1.println();

  //int Kp = askForData("KP").toFloat();
  //int Kd = askForData("KD").toFloat();
  
  motion.setup(0.9, 0.5); // 0.9, 0.5

  Serial1.println("1: print sensors, 2: test distance, 3: test angle, 4: wall follow, 5: hardcoded 6. test MPU");
  mode = askForData("Select mode:").toInt();
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
      break;
    case 3:
      testDriveAngle(-90);
      printMpu();
      break;
    case 4:
      wallFollow();
      break;
    case 5:
      testHardCodedMaze();
      break;
    case 6:
      printMpu();
      delay(200);
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
