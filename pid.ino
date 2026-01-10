int oldError = 0;

float PID (float Kp, float Kd, float input, float target) {
  float error = target - input;

  float proportional = Kp * error
  float derivative = (oldError - error) * Kd

  oldError = error;
  
  return proprtional + derivative
}
