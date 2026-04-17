// Gemini Motion Controller
// Creates a class `Motion()` that exposes functions `driveDistance(mm)` and `driveAngle(deg)`
// You must call `update()` in a loop/ISR

// telling the program these exist
extern volatile int leftEncoderValue;
extern volatile int rightEncoderValue;

extern volatile int leftSensorValue;
extern volatile int rightSensorValue;

extern volatile int encoderDifference;

extern volatile float heading;

void setMotors(int left, int right);
void stop();

void updateDirection(int turnDir);
void updatePosition();
void updatePositionBack();

float pulsesToDistance(int pulses);
float calculateDistancePulses(int mm);
float calculateCellPulses(int cellPercentage, bool braking = false);
float calculateAnglePulses(int deg);

void resetHeadingVariable();

extern const float SENSOR_KP;
extern const float SENSOR_KD;
extern const int LEFT_TARGET;
extern const int RIGHT_TARGET;
// end externs

#define DRIVE_PID_NONE 0
#define DRIVE_PID_LEFT 1
#define DRIVE_PID_RIGHT 2

class Motion {
  private:
      // state: 0=idle 1=driveDistance 2=driveAngle 3=driveCell
      volatile uint8_t state = 0;
      
      // pid variables
      PID _motionPID;
      PID _sensorPID;

      const int DRIVE_SPEED = 50; // 50
      const int TURN_SPEED = 60; // 60
      
      // distance driving variables
      int _distancePulsesToGo = 0;
      int direction = 1;
      int distanceSpeed = DRIVE_SPEED * direction;
      int lastCheckpoint = 0;

      // cell driving variables
      int _cellPulsesToGo = 0;
      int cellDirection = 1;
      int cellSpeed = DRIVE_SPEED * cellDirection;
      uint8_t sensorPidSetting = DRIVE_PID_NONE;

      // angle driving variables
      int _angleDegreesToGo = 0;
      int _anglePulsesToGo = 0;
      int angleSpeed = DRIVE_SPEED; // left wheel's speed when angle driving
      volatile float compensation = 1.0; // the compensation to apply to encoder-based drive to reach 90º
      
  public:
    Motion() : _motionPID(0, 0), _sensorPID(0, 0) {
       _distancePulsesToGo = 0;
       _anglePulsesToGo = 0;
       _cellPulsesToGo = 0;
    }

    void setup(float Kp, float Kd) {
      _motionPID = PID(Kp, Kd);
      _sensorPID = PID(SENSOR_KP, SENSOR_KD);
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

      lastCheckpoint = 0; // reset the checkpoint
      state = 1; // set the state

      // and go!
      _distancePulsesToGo = abs(calculateDistancePulses(distance)); // use ABS to ensure the number is always positive
      setMotors(distanceSpeed, distanceSpeed); // start the motors
    }

    
    void driveCell(int cellPercentage, uint8_t useSensors, bool braking = false) { // 100 is 1 cell etc.
      sensorPidSetting = useSensors;
      
      // reset encoders
      noInterrupts();
      leftEncoderValue = 0;
      rightEncoderValue = 0;
      interrupts();

      // calculate which way to go
      cellDirection = (cellPercentage >= 0) ? 1 : -1; // check if distance is positive - 1 for forward, -1 for back
      cellSpeed = DRIVE_SPEED * cellDirection;

      state = 3; // set the state

      // and go!
      _cellPulsesToGo = abs(calculateCellPulses(cellPercentage, braking)); // use ABS to ensure the number is always positive
      setMotors(cellSpeed, cellSpeed); // start the motors
    }


    // NOTE: This function is written by AI
    // -90 is left 90
    void driveAngle(int deg) {
      // reset encoders
      noInterrupts();
      leftEncoderValue = 0;
      rightEncoderValue = 0;
      interrupts();
      
      _anglePulsesToGo = calculateAnglePulses(abs(deg)); // Always positive
      _angleDegreesToGo = abs(deg) * compensation; // Always positive
      bool turnRight = (deg > 0);
      
      // Determine motor directions
      if (turnRight) {
        angleSpeed = TURN_SPEED * 1;
      } else {
        angleSpeed = TURN_SPEED * -1;
      }
      
      updateDirection(deg); // update positioning direction
      state = 2; // set the state
      resetHeadingVariable(); // reset MPU heading
      // and go!
      setMotors(angleSpeed, -angleSpeed);  
    }
      
    void update() {
      if (state == 1) { // driving distance
        int encoderAvg = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2; // use ABS to ensure the number is always positive
        if (encoderAvg >= _distancePulsesToGo) { // moved enough
          stop();
          state = 0;
        } 
        else {
          float adjustment = _motionPID.calculate(encoderDifference, 0);
          setMotors(distanceSpeed, distanceSpeed + adjustment); 

          /*int distanceTravelled = (int)pulsesToDistance(encoderAvg);
          if ((pulsesToDistance(encoderAvg) > 0) && (distanceTravelled >= lastCheckpoint + 180)) { // moved more than 180
            if (direction == 1) { // moving forward
              lastCheckpoint = distanceTravelled;
              updatePosition();
            } else {
              lastCheckpoint = distanceTravelled;
              updatePositionBack();
            }
          }*/ // this is the loop that updates position
        }
      } 
      else if (state == 2) { // driving angle
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

      else if (state == 3) { // driving cells
        resetHeadingVariable();
        int encoderAvg = (abs(leftEncoderValue) + abs(rightEncoderValue)) / 2; // use ABS to ensure the number is always positive
        if (encoderAvg >= _cellPulsesToGo) { // moved enough
          stop();
          state = 0;
        } 

        else {
          float adjustment = 0;
          if (sensorPidSetting == DRIVE_PID_LEFT) {
             adjustment = _sensorPID.calculate(leftSensorValue, LEFT_TARGET);
          } 
          else if (sensorPidSetting == DRIVE_PID_RIGHT) {
            adjustment = (-1 * _sensorPID.calculate(rightSensorValue, RIGHT_TARGET));
          }
          else {
             adjustment = _motionPID.calculate(heading, 0); // use encoderDifference for encoder-based straight, or heading for MPU-based straight driving
          }
          setMotors(cellSpeed, cellSpeed + adjustment); 
        }
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
