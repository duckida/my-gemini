// Gemini PD Controller
// A `float` function that returns the output of a PD controller
// Inputs: Kp & Kd constants, sensor/encoder input, desired target
// Example: float pid = PID(KP, KD, leftSensorValue, TARGET);

int oldError = 0;

float PID (float Kp, float Kd, float input, float target) {
  float error = target - input;

  float proportional = Kp * error;
  float derivative = (oldError - error) * Kd;

  oldError = error;
  
  return proportional + derivative;
}
