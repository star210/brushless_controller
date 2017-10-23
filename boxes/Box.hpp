#pragma once

#include "Arduino.h"

class Box {
public:
  Box(BrushlessMotorDriver &motor, Sensor &sensor):
  motor(&motor), sensor(&sensor)
  {
    mResetDistance = 0;
    mResetDirection = BMD::Direction::CLOCKWISE;
  }

  void reset() {
    motor->setDirection(BMD::Direction::CLOCKWISE);
    motor->setSpeed(RESET_SPEED);
    motor->enable();
    while (sensor->isOff()) {
      motor->updateBasic(micros());
      delayMicroseconds(1);
    }

    if (mResetDistance > 0) {
      motor->setDistance(mResetDistance);
      motor->turn();
      while(!motor->hasStopped()) {
        motor->update(micros());
        delayMicroseconds(1);
      }
    }
    
    //motor->disable();
  }

  void rotateSides(int nSides) {
    motor->setDistance(90 * nSides);
    motor->turn();
  }

  void update(uint32_t t) {
    motor->update(t);
  }

  void setResetDistance(float d) {
    mResetDistance = d;
  }

  void setResetDirection(BMD::Direction d) {
    mResetDirection = d;
  }

public:
  BrushlessMotorDriver *motor;
  Sensor *sensor;

  float mResetDistance;
  BMD::Direction mResetDirection;
};
