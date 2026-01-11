#include <SerialBT.h>

// --- GLOBAL VARIABLES ---
volatile int leftSensorValue = 0;
volatile int frontSensorValue = 0;
volatile int rightSensorValue = 0;

// --- SETUP ---
void setup() {
  
  // Setup BLE Serial
  SerialBT.setName("PicoW-Gemini"); // Pin 0000
  SerialBT.begin(9600);

  // Initialize components
  initMotors();
}

// --- LOOP ---
void loop() {
  //updateSensors();
  printSensors();
  delay(100);
}

// --- SETUP 1 ---
void setup1() {
  initSensors();
}

// --- LOOP 1 = INTERUPTS ---
void loop1() {
  updateSensors();
}
