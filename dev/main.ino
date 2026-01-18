#include <SerialBT.h>
#include "pid.h"

// --- GLOBAL VARIABLES ---

// Sensor values
volatile int leftSensorValue = 0;
volatile int frontSensorValue = 0;
volatile int rightSensorValue = 0;

// Encoder values
volatile int leftEncoderValue = 0;
volatile int rightEncoderValue = 0;
volatile int encoderDifference = 0; // right - left

// PD controller values
const float KP = 2;
const float KD = 0.2;
const int TARGET = 45;

PID motorPid(KP, KD);

// --- SETUP ---
void setup() {
  
  // Setup BLE Serial
  SerialBT.setName("PicoW-Gemini");
  SerialBT.begin(9600);

  // Initialize components
  initMotors();

  delay(1000);
}

// --- LOOP ---
void loop() {
  testDriveDistance(-100);
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
