#define ACCELERATION 200 
#define ACCELERATION_DISTANCE 90 // degrees
#define MAX_SPEED 15.0f // rpm

#define RESET_SPEED 1.0 // rpm

#define TOP_BOX_RESET_DISTANCE -10
#define TOP_BOX_RESET_DIRECTION BMD::Direction::CLOCKWISE

#define BOTTOM_BOX_RESET_DISTANCE 10
#define BOTTOM_BOX_RESET_DIRECTION BMD::Direction::ANTICLOCKWISE

#define SLEEP_TIME 5 // seconds

#define CYCLES_PER_RESET 10

#define ROTATIONS 2160  // degrees 

//////////////////////////////////////////////////////////////////////

#include "BrushlessMotor.hpp"
#include "Sensor.hpp"
#include "Box.hpp"

void sleep(float seconds) {
  seconds *= 10;
  for (int i = 0; i < 2 * seconds; i++) delay(100 * 32);
}

Sensor topSensor(A2, A3, A4);
Sensor bottosensor(A5, A1, A0);

BrushlessMotorDriver topMotor(3, 5, 6, 4);
BrushlessMotorDriver bottomMotor(11, 10, 9, 8);

Box topBox(topMotor, topSensor);
Box bottomBox(bottomMotor, bottosensor);


void updateMotors() {
  static uint32_t t;
  while(!topMotor.hasStopped() || !bottomMotor.hasStopped()) {
    t = micros();
    topMotor.update(t);
    bottomMotor.update(t);
  }
}

void setup() {
  Serial.begin(115200);
  setPwmFrequency32k();

}

void loop() {
  
  topBox.setResetDistance(TOP_BOX_RESET_DISTANCE);
  topBox.setResetDirection(TOP_BOX_RESET_DIRECTION);
  topBox.reset();

  bottomBox.setResetDistance(BOTTOM_BOX_RESET_DISTANCE);
  bottomBox.setResetDirection(BOTTOM_BOX_RESET_DIRECTION);
  bottomBox.reset();

  for (int c = 0; c < CYCLES_PER_RESET; c++) {
    setMillis(0);

    // FIRST PHASE
    //////////////////////////////////////////////////////////

    sleep(SLEEP_TIME);
    
    topMotor.setDirection(BMD::Direction::CLOCKWISE);
    topMotor.setDistance(1530);
    topMotor.turn();
  
    updateMotors();
    sleep(SLEEP_TIME);

    // SECOND PHASE
    //////////////////////////////////////////////////////////
  
    topMotor.setDirection(BMD::Direction::ANTICLOCKWISE);
    topMotor.setDistance(ROTATIONS + 90);
    topMotor.turn();
  
    bottomMotor.setDirection(BMD::Direction::CLOCKWISE);
    bottomMotor.setDistance(ROTATIONS + 90);
    bottomMotor.turn();
  
    updateMotors();
    sleep(SLEEP_TIME);

    // THIRD PHASE
    //////////////////////////////////////////////////////////
  
    topMotor.setDirection(BMD::Direction::CLOCKWISE);
    topMotor.setDistance(ROTATIONS + 90);
    topMotor.turn();
  
    bottomMotor.setDirection(BMD::Direction::ANTICLOCKWISE);
    bottomMotor.setDistance(ROTATIONS + 90);
    bottomMotor.turn();
  
    updateMotors();
    sleep(SLEEP_TIME);

  }
  setMillis(0);

}
extern volatile unsigned long timer0_millis;

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
