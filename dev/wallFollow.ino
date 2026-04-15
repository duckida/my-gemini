enum WallFollowState {
  FOLLOW_WALL,
  SP_TO_CENTER,
  TURNING_LEFT,
  DRIVING_TO_NEXT_CELL,
  TURNING_RIGHT
}; 

WallFollowState wfState = FOLLOW_WALL;
int angleToGo = 0;

void wallFollow() {
  switch(wfState) {
    case FOLLOW_WALL: { // just looking around
      if (leftSensorValue <= LEFT_GAP) { // gap on the left
        motion.driveDistance(150); // start driving to the middle of the cell
        wfState = SP_TO_CENTER; 
        on(MEZZ_RIGHT_LED);
      }

      else if (frontSensorValue >= FRONT_WALL) { // wall in front
        delay(200);
        motion.driveAngle(-90); // start turning right
        wfState = TURNING_RIGHT;
      }

      else {
        float adj = sensorPid.calculate(leftSensorValue, LEFT_TARGET);
        setMotors(60 - adj, 60 + adj);
      }
      
      break;
    }

    case SP_TO_CENTER: {
      if (motion.completed()) { // once driving is done
        delay(200);
        motion.driveAngle(90); // turn left
        wfState = TURNING_LEFT;
        off(MEZZ_RIGHT_LED);
      }
      
      break;
    }

    case TURNING_LEFT: {
      if (motion.completed()) { // once driving is done
        delay(200);
        wfState = FOLLOW_WALL; // go back to exploring
      }
      
      break;
    }

    case TURNING_RIGHT: {
      if (motion.completed()) { // once driving is done
        delay(200);
        Serial.println("Done turning right");
        wfState = FOLLOW_WALL; // go back to exploring
      }
      
      break;
    }
  }
}
