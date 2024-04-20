#ifndef LOCAL_MOTORS_H
#define LOCAL_MOTORS_H

#include "motor_settings.h"
#include "LocalMotorControl.h"
#include "Motors.h"

IMotorControl** rpcMotors;

void enableMotor(int idx) {
  rpcMotors[idx]->enable();
}

void disableMotor(int idx) {
  rpcMotors[idx]->disable();
}

void updateMotorSpeed(int idx, double delta) {
  rpcMotors[idx]->updateSpeed(delta);
}

void setMotorSpeed(int idx, double speed) {
  rpcMotors[idx]->setSpeed(speed);
}

class LocalMotors : public Motors {
  IMotorControl* motors[NO_MOTORS];
public:
  LocalMotors(boolean enableRpc = false) {
    if (enableRpc) {
        rpcMotors = motors;
        RPC.bind("enableMotor", enableMotor);
        RPC.bind("disableMotor", disableMotor);
        RPC.bind("updateMotorSpeed", updateMotorSpeed);
        RPC.bind("setMotorSpeed", setMotorSpeed);
    }
  }
  IMotorControl* get(int idx) {
    return motors[idx];
  }

  void setup() {
    motors[X_MOTOR] = new LocalMotorControl(53, 51); // , 49
    motors[Y_MOTOR] = new LocalMotorControl(52, 50); // , 48
  }

  void loop() {
    for (int i = 0; i < NO_MOTORS; ++i) {
      motors[i]->run();
    }
  }
};
#endif