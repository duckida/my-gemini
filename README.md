# my-gemini
This repository contains the code for my micromouse (maze-solving robot) Totoro, based on the UKMARS Gemini platform (with an added MPU6050 and HC-05).

<img width="1600" height="891" alt="image" src="https://github.com/user-attachments/assets/5685cf13-c2d9-48f1-90b8-8b2578c68f86" />


The code is broken down into separate files which contain a number of functions each:
- gemini.ino - the main Arduino code
- buttons.ino	- allows use of pushbuttons
- floodfill.ino	- contains maze-solving algorithm & maze driving loop
- motion.h - drives motors a particular amount using gyroscope & encoders
- pid.h	- PID controller class
- telemetry.ino - sends telemetry in JSON to a mobile maze viewer app
- config.ino - allows configuration by Serial
- motors.ino - contains low-level motor functions
- positioning.ino	- allows the robot to know where in the maze it is
- tests.ino - tests for the robot
- encoders.ino - updates the encoder counts
- leds.ino - allows the use of status LEDs
- mpu.ino	- updates heading using the gyroscope
- sensors.ino	- Reads the phototransistor-based wall sensors
- wallFollow.ino - simple wall following algorithm

The robot has successfully completed a 16x16 maze in the UKMARS competition 2026!



