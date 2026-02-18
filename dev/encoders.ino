// Gemini Encoders
// Step 1. `initEncoders();` which sets up the pins and attaches interrupts
// The, use variables `leftEncoderValue` and `rightEncoderValue` to access
// Use `printEncoders();` to output the values in format L R
// Includes function `calculateDistance(mm)` to calculate how many pulses to travel
// and `resetEncoders()` to set both values to 0

#define ENCODER_LEFT_A 8
#define ENCODER_LEFT_B 9

#define ENCODER_RIGHT_A 7
#define ENCODER_RIGHT_B 6

void initEncoders() {
  // Set pin mode
  pinMode(ENCODER_LEFT_A, INPUT_PULLUP); 
  pinMode(ENCODER_LEFT_B, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_A, INPUT_PULLUP); 
  pinMode(ENCODER_RIGHT_B, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A), leftEncoderISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_B), leftEncoderISR_B, CHANGE);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A), rightEncoderISR_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_B), rightEncoderISR_B, CHANGE);
}

// ISRs to increment or decrement counts based on direction
// --- LEFT ENCODER ---
void leftEncoderISR_A() {
  // When A changes: if A != B, we moved Forward (++)
  if (digitalRead(ENCODER_LEFT_A) != digitalRead(ENCODER_LEFT_B)) {
    leftEncoderValue++;
  } else {
    leftEncoderValue--;
  }
  encoderDifference = rightEncoderValue - leftEncoderValue;
}

void leftEncoderISR_B() {
  // When B changes: if A == B, we moved Forward (++)
  if (digitalRead(ENCODER_LEFT_A) == digitalRead(ENCODER_LEFT_B)) {
    leftEncoderValue++;
  } else {
    leftEncoderValue--;
  }
  encoderDifference = rightEncoderValue - leftEncoderValue;
}

// --- RIGHT ENCODER ---
void rightEncoderISR_A() {
  if (digitalRead(ENCODER_RIGHT_A) != digitalRead(ENCODER_RIGHT_B)) {
    rightEncoderValue++;
  } else {
    rightEncoderValue--;
  }
  encoderDifference = rightEncoderValue - leftEncoderValue;
}

void rightEncoderISR_B() {
  if (digitalRead(ENCODER_RIGHT_A) == digitalRead(ENCODER_RIGHT_B)) {
    rightEncoderValue++;
  } else {
    rightEncoderValue--;
  }
  encoderDifference = rightEncoderValue - leftEncoderValue;
}

// --- UTILITY FUNCTIONS ---

void printEncoders() {
  Serial1.print(leftEncoderValue);
  Serial1.print(" ");
  Serial1.print(rightEncoderValue);
  Serial1.println();
}

float calculateDistancePulses(int mm) {
/* Wheel diameter: 32mm
Wheel circumference: = π x 32 = 100.5309649149
Encoder pulses per full rotation of backshaft: 12
Motor gear ratio: 1:50
Encoder pulses per full rotation of wheel: 12 * 50 = 600
So every 600 pulses we go 100.5309mm
Pulses per mm: 600 / 100.53096 = 5.9683106577
*/
  float pulsesPerMm = 5.9683106577;
  float compensation = 0.853; // human compensation
  return (mm * pulsesPerMm * compensation); 
}

float calculateAnglePulses(int deg) {
  /*
  pulsesPerMm = 5.9683106577;
  compensation = 0.853;

  Inverse of count per mm is mm per count! (5.968... * 0.853) is 0.1964...
  Wheel separation = 74.5mm

  DEG_PER_COUNT = (360.0 * MM_PER_COUNT) / (PI * WHEEL_SEPARATION);

  DEG_PER_COUNT = (360.0 * 0.1964) / (3.1415926 * 74.5);
  = 0.3020910413

  COUNT_PER_DEG = 1 / DEG_PER_COUNT
  1 / 0.3020910413 = 3.3102603629
  */
  float compensation = 0.94; // human compensation
  return deg * 3.3102603629 * compensation;
}

void resetEncoders() {
  leftEncoderValue = 0;
  rightEncoderValue = 0;
}
