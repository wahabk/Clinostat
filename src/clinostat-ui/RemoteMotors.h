#ifndef REMOTE_MOTORS_H
#define REMOTE_MOTORS_H

#include <RPC.h>

#include "motor_settings.h"
#include "LocalMotorControl.h"
#include "Motors.h"

class RpcMotorControl : public IMotorControl {
  int idx;

public:
  RpcMotorControl(int idx) {
    this->idx = idx;
  }

  bool run() {
    // nop
  }

  void updateSpeed(float deltaRpm) {
    RPC.call("updateMotorSpeed", idx, deltaRpm);
  }

  void setSpeed(float rpm) {
    RPC.call("setMotorSpeed", idx, rpm);
  }

  void enable() {
    RPC.call("enableMotor", idx);
  }

  void disable() {
    RPC.call("disableMotor", idx);
  }

  void setEnablePin(uint8_t pin = 0xff) {
  }
};


class RemoteMotors : public Motors {
  IMotorControl* motors[NO_MOTORS];
public:
  IMotorControl* get(int idx) {
    return motors[idx];
  }

  void setup() {
    motors[X_MOTOR] = new RpcMotorControl(X_MOTOR);
    motors[Y_MOTOR] = new RpcMotorControl(Y_MOTOR);
  }

  void loop() {
    // nop really
  }
};
#endif