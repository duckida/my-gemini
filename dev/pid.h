// Gemini PD Controller
// A class that returns the output of a PD controller
// Initialize: Kp & Kd constants 
// Calculate: sensor/encoder input, desired target
// Example: PID motorPid(Kp, Kd)
// motorPid.calculate(encoderDifference, 0)
class PID {
  private: 
    float _oldError = 0;
    float _KP;
    float _KD;

  public:

    PID(float Kp, float Kd) {
       _KP = Kp;
       _KD = Kd;
      }
      
    float calculate(float input, float target) {
      float error = target - input;

      float proportional = _KP * error;
      float derivative = (error - _oldError) * _KD;

      _oldError = error;
  
      return proportional + derivative;
    }
};
