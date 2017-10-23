#include "Control.hpp"
#include "BrushlessMotor.hpp"

typedef BrushlessMotorDriver BMD;

BMD motor(3, 5, 6, 4);
Control<BMD> control(&motor);

void setup() {
  Serial.begin(115200);
  setPwmFrequency32k();

  ////////////////////////////////////////////////////////////
  // THIS IS WHERE YOU SET THE SEQUENCE
  // The idea is that the sequence can be read easily
  // Each instruction is executed sequentially
  // Some instructions (like continueFor) execute for a given time
  
  control.add(&BMD::setClockwise);
  control.add(&BMD::enable);
  control.add(&BMD::turn, 10.0f); //rpm
  control.add(&BMD::continueFor, 5.0f); // seconds
  control.add(&BMD::setAntiClockwise);
  control.add(&BMD::turn, 20.0f); //rpm
  control.add(&BMD::continueFor, 2.0f); // seconds
  control.add(&BMD::disable);
  control.add(&BMD::sleep, 2.0f); // seconds

  // other options:
  // control.add(&BMD::stop);
  ////////////////////////////////////////////////////////////

  control.start();
}

void loop() {
  static float t;
 
  do {
    t = millis() / 64000.0;
  } while(control.run(t));
  
  control.start();
  setMillis(0);
}

extern volatile unsigned long timer0_millis;

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
