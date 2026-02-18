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
const int LEFT_GAP = 5;
const int FRONT_WALL = 40;
const int RIGHT_GAP = 5;

// Encoder values
volatile int leftEncoderValue = 0;
volatile int rightEncoderValue = 0;
volatile int encoderDifference = 0; // right - left

// PD controller values
const float SENSOR_KP = 0.2;
const float SENSOR_KD = 0.3;
const int TARGET = 25;

PID sensorPid(SENSOR_KP, SENSOR_KD);
PID motorPid(0.5, 0);

// Timer setup
RPI_PICO_Timer ITimer(0);

// Motion controller
Motion motion; 
// note: 0.9 and 0.5 are good Kp/Kd value for motion


// --- SETUP ---
void setup() {

  // Setup BLE Serial
  Serial1.begin(57600);

  // Initialize components
  initMotors();
  initButtons();
  initLeds();
  initEncoders();

  delay(2000);

  // start timer
  float frequency = 500.0f;
  ITimer.attachInterrupt(frequency, TimerHandler);

  // get config details
  Serial1.println("------GEMINI------");

  //int Kp = askForData("KP").toFloat();
  //int Kd = askForData("KD").toFloat();
  
  motion.setup(0.9, 0.5); // 0.9, 0.5
}

// -- TIMER RUNNER --
bool TimerHandler(struct repeating_timer *t)
{
  motion.update();
  return true; // Return true to keep the timer repeating
}

void loop() {
  //printSensors();
  wallFollow();
  //testDriveAngle(-90);
  //delay(200);
};

void setup1() {
  initSensors();
}

void loop1() {
  updateSensors();
};
