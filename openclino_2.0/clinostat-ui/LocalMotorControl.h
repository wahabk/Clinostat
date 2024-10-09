#ifndef LOCAL_MOTOR_CONTROL_H
#define LOCAL_MOTOR_CONTROL_H

#include <AccelStepper.h>

#include "IMotorControl.h"
#include "motor_settings.h"

// Control single motor.
class LocalMotorControl : public IMotorControl {

private:
  // stepper itself
  AccelStepper* stepper;

  // if enambled
  boolean isEnabled = false;

  float speed = 0.0;

  uint8_t enablePin = 0xff;

public:
  LocalMotorControl(int pulsePin, int directionPin, uint8_t enablePin = 0xff) {
    this->stepper = new AccelStepper(AccelStepper::DRIVER, pulsePin, directionPin);
    stepper->setCurrentPosition(0);
    stepper->setMaxSpeed(30 * stepsPerRevolution / 60.0);
    stepper->setAcceleration(5.0);
    stepper->setSpeed(speed);

    this->enablePin = enablePin;
    setEnablePin(enablePin);
  }


  bool run() {
    if (isEnabled) {
      stepper->runSpeed();
    }

    return isEnabled;
  }

  void updateSpeed(float delta) {
    speed += delta;
    setSpeed(speed);
  }

  void setSpeed(float speed) {
    this->speed = speed;
    stepper->setSpeed(speed * stepsPerRevolution/60.0);
  }


  void enable() {
    isEnabled = true;
    if (enablePin != 0xff) {
      stepper->enableOutputs();
    }
  }

  void disable() {
    isEnabled = false;
    if (enablePin != 0xff) {
      stepper->disableOutputs();
    }
  }

  void setEnablePin(uint8_t pin = 0xff) {
    stepper->setEnablePin(pin);
    if (pin != 0xff) {
      stepper->setPinsInverted(false, false, true);
    }
  }
};

#endif