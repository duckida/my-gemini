// Gemini Encoders
// Step 1. `initEncoders();` which sets up the pins and attaches interrupts
// The, use variables `leftEncoderValue` and `rightEncoderValue` to access
// Use `printEncoders();` to output the values in format L R
// Includes function `calculateDistance(mm)` to calculate how many pulses to travel

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
}

void leftEncoderISR_B() {
  // When B changes: if A == B, we moved Forward (++)
  if (digitalRead(ENCODER_LEFT_A) == digitalRead(ENCODER_LEFT_B)) {
    leftEncoderValue++;
  } else {
    leftEncoderValue--;
  }
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
  SerialBT.print(leftEncoderValue);
  SerialBT.print(" ");
  SerialBT.print(rightEncoderValue);
  SerialBT.println();

  encoderDifference = rightEncoderValue - leftEncoderValue;
}

float calculatePulses(int mm) {
/* Wheel diameter: 32mm
Wheel circumference: = π x 32 = 100.5309649149
Encoder pulses per full rotation of backshaft: 12
Motor gear ratio: 1:50
Encoder pulses per full rotation of wheel: 12 * 50 = 600
So every 600 pulses we go 100.5309mm
Pulses per mm: 600 / 100.53096 = 5.9683106577
*/
  float pulsesPerMm = 5.9683106577;
  return (mm * pulsesPerMm); 
}
