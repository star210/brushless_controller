#pragma once

#include "Arduino.h"

inline void setPwmFrequency32k() {
	TCCR0B = TCCR0B & 0b11111000 | 0x01;
	TCCR1B = TCCR1B & 0b11111000 | 0x01;
	TCCR2B = TCCR2B & 0b11111000 | 0x01;
}

class BrushlessMotor {
public:
  enum class Direction { CLOCKWISE, ANTICLOCKWISE };

  struct Pins {
    Pins(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t enable):
    enable(enable) {
      out[0] = p1;
      out[1] = p2;
      out[2] = p3;
    }
    uint8_t enable;
    uint8_t out[3];
  };
  
public:
	BrushlessMotor(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t enable);
  BrushlessMotor(Pins pins);

	void enable(bool on=true) const;
	void disable() const { enable(false); }

  void setDirection(Direction direction);

  void stop();
	
protected:
  void construct();
	void drive();
	
protected:
  Pins mPins;
	uint16_t mPhases[3];
	int8_t mStep;

};


class BrushlessMotorDriver : public BrushlessMotor {
public:
	BrushlessMotorDriver(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t enable);
  BrushlessMotorDriver(Pins pins);
  
	void setSpeed(float rpm);

	bool turn(float t);

	bool turn(float t, float rpm) {
		setSpeed(rpm);
		return turn(t);
	}
	
	bool turn(float t, BrushlessMotor::Direction direction) {
		setDirection(direction);
		return turn(t);
	}

	bool turn(float t, float rpm, BrushlessMotor::Direction direction) {
		setSpeed(rpm);
		setDirection(direction);
		return turn(t);
	}

	void update(uint32_t currentMicros);

  bool continueFor(float currentTime, float seconds);

  bool setClockwise(float t) {
    setDirection(BrushlessMotor::Direction::CLOCKWISE);
    return true;
  }

  bool setAntiClockwise(float t) {
    setDirection(BrushlessMotor::Direction::ANTICLOCKWISE);
    return true;
  }

  bool disable() {
    BrushlessMotor::disable();
    return true;
  }

  bool enable() {
    BrushlessMotor::enable();
    return true;
  }

  bool stop() {
    
  }

  bool sleep(float t, float seconds) {
    seconds *= 10;
    for (int i = 0; i < 2 * seconds; i++) delay(100 * 32);
    return true;
  }

protected:
	uint32_t mLastDrive;
	uint32_t mDriveDelay;

  float mTurnStartTime;
};
