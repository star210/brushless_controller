#include "Control.hpp"
#include "BrushlessMotor.hpp"

#define NUM_REPEATS 5
#define SLEEP_TIME 3.0f // seconds

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
  
  float t = 0.2;

  control.add(&BMD::setClockwise);
  control.add(&BMD::enable);
  control.add(&BMD::turn, 10.0f); //rpm
  control.add(&BMD::continueFor, t); // seconds
  
  control.add(&BMD::setAntiClockwise);
  control.add(&BMD::turn, 10.0f); //rpm
  control.add(&BMD::continueFor, t * 2.0f); // seconds

  control.add(&BMD::setClockwise);
  control.add(&BMD::turn, 10.0f); //rpm
  control.add(&BMD::continueFor, t); // seconds
  control.add(&BMD::disable);
  control.add(&BMD::sleep, 0.2f); // seconds


  // other options:
  // control.add(&BMD::stop);
  ////////////////////////////////////////////////////////////

  control.start();
}

void loop() {
  static float t;

  for (int n = 0; n < NUM_REPEATS; n++) {
    do {
      t = millis() / 64000.0;
    } while(control.run(t));
    
    control.start();
    setMillis(0);
  }

  for (int i = 0; i < 64; i++) 
    delay(1000 * SLEEP_TIME);
  
}

extern volatile unsigned long timer0_millis;

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
}
