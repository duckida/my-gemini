#define LEFT_BUTTON 15
#define RIGHT_BUTTON 14

void initButtons() {
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
}

void waitForButton(int pin) {
  while (digitalRead(pin) == HIGH); 
}

