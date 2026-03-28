// Gemini Motion Controller
// Creates a class `Motion()` that exposes functions `driveDistance(mm)` and `driveAngle(deg)`
// You must call `update()` in a loop/ISR

extern volatile int leftEncoderValue;
extern volatile int rightEncoderValue;
extern volatile int encoderDifference;
extern volatile float heading;
void setMotors(int left, int right);
void stop();
void resetHeadingVariable();
float calculateDistancePulses(int mm);
float calculateAnglePulses(int deg);

class Motion {
  private:
      // state: 0=idle 1=driveDistance 2=driveAngle
      volatile int state = 0;
      
      // pid variables
      PID _motionPID;

      const int DRIVE_SPEED = 60;
      const int TURN_SPEED = 60;
      
      // distance driving variables
      int _distancePulsesToGo = 0;
      int direction = 1;
      int distanceSpeed = DRIVE_SPEED * direction;

      // angle driving variables
      int _angleDegreesToGo = 0;
      int _anglePulsesToGo = 0;
      int angleSpeed = DRIVE_SPEED; // left wheel's speed when angle driving
      volatile float compensation = 1.0; // the compensation to apply to encoder-based drive to reach 90º
      
  public:
    Motion() : _motionPID(0, 0) {
       _distancePulsesToGo = 0;
       _anglePulsesToGo = 0;
    }

    void setup(float Kp, float Kd) {
      _motionPID = PID(Kp, Kd);
    }

    // Distance driving

    void driveDistance(int distance) {
      // reset encoders
      noInterrupts();
      leftEncoderValue = 0;
      rightEncoderValue = 0;
      interrupts();

      // calculate which way to go
      direction = (distance >= 0) ? 1 : -1; // check if distance is positive - 1 for forward, -1 for back
      distanceSpeed = DRIVE_SPEED * direction;
      
      state = 1; // set the state

      // and go!
      _distancePulsesToGo = abs(calculateDistancePulses(distance)); // use ABS to ensure the number is always positive
      setMotors(distanceSpeed, distanceSpeed); // start the motors
    }

    // NOTE: This function is written by AI
    void driveAngle(int deg) {
      // reset encoders
      noInterrupts();
      leftEncoderValue = 0;
      rightEncoderValue = 0;
      interrupts();
      
      _anglePulsesToGo = calculateAnglePulses(abs(deg)); // Always positive
      _angleDegreesToGo = abs(deg) * compensation; // Always positive
      bool turnRight = (deg < 0);
      
      // Determine motor directions
      if (turnRight) {
        angleSpeed = TURN_SPEED * 1;
      } else {
        angleSpeed = TURN_SPEED * -1;
      }

      state = 2; // set the state
      resetHeadingVariable(); // reset MPU heading
      // and go!
      setMotors(angleSpeed, -angleSpeed);  
    }
      
    void update() {
      if (state == 1) { // driving distance
        int encoderAvg = abs(leftEncoderValue + rightEncoderValue) / 2; // use ABS to ensure the number is always positive
        if (encoderAvg >= _distancePulsesToGo) {
          stop();
          state = 0;
        } else {
          float adjustment = _motionPID.calculate(encoderDifference, 0);
          setMotors(distanceSpeed, distanceSpeed + adjustment); 
        }
      } else if (state == 2) { // driving angle
        /*if (abs(heading) >= _angleDegreesToGo) {
          stop();
          state = 0;
        }*/ // MPU method
 
        long totalMoved = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2;
        if (totalMoved >= _anglePulsesToGo) {
          stop();
          compensation = abs(heading) / abs(_angleDegreesToGo);
          state = 0;
        } // Encoder method
      }
    }

    bool completed() {
      if (state == 0) {
        return true;
      } else {
        return false;
      }
    }
};
