#include "BrushlessMotor.hpp"

#define SINE_WAVE_LENGTH 360
const uint8_t SINE_WAVE[SINE_WAVE_LENGTH] = { 128, 132, 136, 140, 143, 147, 151, 155, 159, 162, 166, 170, 174, 178, 181, 185, 189, 192, 196, 200, 203, 207, 211, 214, 218, 221, 225, 228, 232, 235, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 248, 249, 250, 250, 251, 252, 252, 253, 253, 253, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 253, 252, 252, 251, 250, 250, 249, 248, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 235, 232, 228, 225, 221, 218, 214, 211, 207, 203, 200, 196, 192, 189, 185, 181, 178, 174, 170, 166, 162, 159, 155, 151, 147, 143, 140, 136, 132, 128, 124, 120, 116, 113, 109, 105, 101, 97, 94, 90, 86, 82, 78, 75, 71, 67, 64, 60, 56, 53, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 53, 56, 60, 64, 67, 71, 75, 78, 82, 86, 90, 94, 97, 101, 105, 109, 113, 116, 120, 124 };


BrushlessMotor::BrushlessMotor(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t enable):
	mPins(in1, in2, in3, enable) {
    construct();
}

BrushlessMotor::BrushlessMotor(Pins pins):
  mPins(pins) {
  construct();
}
	
void BrushlessMotor::construct() {
	uint16_t phaseShift = SINE_WAVE_LENGTH / 3;
	
	for (uint8_t i = 0; i < 3; i++) {
		pinMode(mPins.out[i], OUTPUT);
		mPhases[i] = phaseShift * i;
	}

	pinMode(mPins.enable, OUTPUT);
	
	setPwmFrequency32k();

	setDirection(Direction::CLOCKWISE);

}

void BrushlessMotor::enable(bool on) const {
	digitalWrite(mPins.enable, on);
}

void BrushlessMotor::setDirection(Direction direction) {
	switch (direction) {
	case Direction::CLOCKWISE:
		mStep = 1;
		break;
	case Direction::ANTICLOCKWISE:
		mStep = -1;
		break;
	}
}

void BrushlessMotor::drive() {
	for (uint8_t i = 0; i < 3; i++) {
		analogWrite(mPins.out[i], SINE_WAVE[mPhases[i]]);
		mPhases[i] = (mPhases[i] + SINE_WAVE_LENGTH + mStep) % SINE_WAVE_LENGTH;
	}
}


BrushlessMotorDriver::BrushlessMotorDriver(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t enable):
	BrushlessMotor(p1, p2, p3, enable), mLastDrive(0) {
	
}

BrushlessMotorDriver::BrushlessMotorDriver(BrushlessMotor::Pins pins):
  BrushlessMotor(pins), mLastDrive(0) {
  
}

bool BrushlessMotorDriver::turn(float t) {
  mTurnStartTime = t;
  return true;
}

void BrushlessMotorDriver::update(uint32_t currentMicros) {

	if (currentMicros - mLastDrive > mDriveDelay) {
    drive();
    mLastDrive = currentMicros;
	}
}

void BrushlessMotorDriver::setSpeed(float rpm) {
  mDriveDelay = static_cast<uint32_t>(15238.09 / rpm); 
}

bool BrushlessMotorDriver::continueFor(float currentTime, float seconds) {
  uint32_t currentMicros = static_cast<uint32_t>(currentTime * 1000000);
  update(currentMicros);
  return currentTime > mTurnStartTime + seconds;
}


