#ifndef CLINOSTAT_COTROL_H
#define CLINOSTAT_COTROL_H

#import "Motors.h"
#import "motor_settings.h"

// works with motor controls and provides a higher level interface 
class ClinostatControl {
  // these are the speeds of the frames in RPM unlike the speeds of the motors. The motor speeds will need to be adjusted taking 
  // into account the pulleys sizes.
  float xSpeed = 9;

  float ySpeed = 20;
  
  IMotorControl* xMotor;
  
  IMotorControl* yMotor;

  public:

  ClinostatControl(Motors* motors) {
    this->xMotor = motors->get(X_MOTOR);
    this->yMotor = motors->get(Y_MOTOR);
  }

  // set speeds and compensates for Y
  void setSpeed(float xSpeed, float ySpeed) {
    float adjustedXSpeed = xSpeed*xRatio;
    float adjustedYSpeed = ySpeed*yRatio;
    xMotor->setSpeed(adjustedXSpeed);
    yMotor->setSpeed(adjustedYSpeed-adjustedXSpeed);
    this->xSpeed = xSpeed;
    this->ySpeed = ySpeed;
  }

  void setXSpeed(float targetSpeed) {
    xMotor->setSpeed(targetSpeed*xRatio);
    xSpeed = targetSpeed;
  }

  void startX() {
    xMotor->enable();
  }

  void stopX() {
    xMotor->disable();
  }

  float getXSpeed() {
    return xSpeed;
  }

 void setYSpeed(float targetSpeed) {
    yMotor->setSpeed(targetSpeed*yRatio);
    ySpeed = targetSpeed;
  }

  void startY() {
    yMotor->enable();
  }

  void stopY() {
    yMotor->disable();
  }

  float getYSpeed() {
    return ySpeed;
  }

  void start() {
    if (xSpeed != 0.0 || ySpeed != 0.0) {
      startX();
      startY();
    }
  }

  void stop() {
    stopX();
    stopY();
  }

  boolean canRun() {
    return xSpeed != 0.0 || ySpeed != 0.0;
  }
};

#endif