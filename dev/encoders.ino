// Gemini Encoders
// Step 1. `initEncoders();` which sets up the pins and attaches interrupts
// The, use variables `leftEncoderValue` and `rightEncoderValue` to access
// Use `printEncoders();` to output the values in format L R

#define ENCODER_LEFT_A 8
#define ENCODER_LEFT_B 9

#define ENCODER_RIGHT_A 6
#define ENCODER_RIGHT_B 7

void initEncoders() {
  // Set pin mode
  pinMode(ENCODER_LEFT_A, INPUT_PULLUP); 
  pinMode(ENCODER_LEFT_B, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT_A, INPUT_PULLUP); 
  pinMode(ENCODER_RIGHT_B, INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT_A), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT_A), rightEncoderISR, RISING);
}

// ISRs to increment or decrement counts based on direction
void leftEncoderISR() {
  if (digitalRead(ENCODER_LEFT_B) == LOW) {
    leftEncoderValue++;
  } else {
    leftEncoderValue--;
  }
}

void rightEncoderISR() {
  if (digitalRead(ENCODER_RIGHT_B) == LOW) {
    rightEncoderValue++;
  } else {
    rightEncoderValue--;
  }
}

void printEncoders() {
  SerialBT.print(leftEncoderValue);
  SerialBT.print(" ");
  SerialBT.print(rightEncoderValue);
  SerialBT.println();
}

