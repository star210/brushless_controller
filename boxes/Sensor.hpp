#include "Arduino.h"

class Sensor {
public:
  Sensor(uint8_t signal, uint8_t vcc, uint8_t gnd):
  mSignalPin(signal) {
    pinMode(signal, INPUT_PULLUP);
    pinMode(vcc, OUTPUT);
    pinMode(gnd, OUTPUT);
    digitalWrite(vcc, HIGH);
    digitalWrite(gnd, LOW);
  }

  bool isOn() const {
    return digitalRead(mSignalPin) == HIGH;
  }

  bool isOff() const {
    return !isOn();
  }

protected:
  uint8_t mSignalPin;  
};
