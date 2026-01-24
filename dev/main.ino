#include <SerialBT.h>
#include "pid.h"

// --- GLOBAL VARIABLES ---

// Sensor values
volatile int leftSensorValue = 0;
volatile int frontSensorValue = 0;
volatile int rightSensorValue = 0;

const int LEFT_GAP = 10;
const int FRONT_WALL = 70;
const int RIGHT_GAP = 10;

// Encoder values
volatile int leftEncoderValue = 0;
volatile int rightEncoderValue = 0;
volatile int encoderDifference = 0; // right - left

// PD controller values
const float KP = 2;
const float KD = 0.2;
const int TARGET = 35;

PID sensorPid(KP, KD);
PID motorPid(KP, KD);

// --- SETUP ---
void setup() {

  // Setup BLE Serial
  SerialBT.setName("PicoW-Gemini");
  SerialBT.begin(9600);

  // Initialize components
  initMotors();
  initButtons();
  initLeds();

  waitForButton(15);
}

// --- LOOP ---
void loop() {
  //testDriveAngle(90);
  //testWallFollow();
  // wallFollower();
  //testSensorPD(TARGET);
  printSensors();
  testWalls();
}

// --- SETUP 1 ---
void setup1() {
  initSensors();
  initEncoders();
}

// --- LOOP 1 = INTERUPTS ---
void loop1() {
  updateSensors();
}
