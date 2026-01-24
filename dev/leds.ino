#define LEFT_LED = 20;
#define MIDDLE_LED = 19;
#define RIGHT_LED = 18;

void initLeds() {
  pinMode(LEFT_LED, OUTPUT);
  pinMode(MIDDLE_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
}

void on(int pin) {
  digitalWrite(pin, HIGH);
}

void off(int pin) {
  digitalWrite(pin, LOW);
}
