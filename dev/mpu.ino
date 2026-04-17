// Gemini MPU6050 module
// `initMpu()` sets up the pins and I2C
// The, use `updateMpu()` in an interrupt or core 2
// Value is updated to float `heading`
// To reset the heading from core 1, set `requestHeadingreset = true`
// To reset the heading from core 2, call `resetHeading()`

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float gyroBiasZ = 0.0;
unsigned long lastTime = 0;

void initMpu() {
  Wire1.setSDA(10);
  Wire1.setSCL(11);
  Wire1.begin();
  Wire1.setClock(400000);

  if (!mpu.begin(0x68, &Wire1)) {
    Serial1.println("MPU6050 not found!");
    while (1) delay(10);
  }

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
  Serial1.println("MPU INIT!");
}

void resetHeading() {
  int samples = 500;
  double sum = 0;
  for (int i = 0; i < samples; i++) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);
    sum += gyro.gyro.z;
    delay(2);
  }
  gyroBiasZ = sum / samples;
  heading = 0.0;
  lastTime = micros(); 
}

void resetHeadingVariable() {
  heading = 0.0;
}

void updateMpu() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  unsigned long now = micros();
  float dt = (now - lastTime) / 1000000.0;
  lastTime = now;

  float rateZ = (gyro.gyro.z - gyroBiasZ) * (180.0 / PI);
  if (abs(rateZ) > 0.3) {
    // We calculate the movement once...
    float delta = rateZ * dt; 
    
    // ...and apply it to both "buckets"
    heading += delta;       
    globalHeading += delta; 
  }
}

void printMpu() {
  Serial1.print(heading);
  Serial1.print("º");
  Serial1.println();
}
