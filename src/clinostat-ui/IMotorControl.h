#ifndef I_MOTOR_CONTROL_H
#define I_MOTOR_CONTROL_H
// a motor control interface
class IMotorControl {
  public:
  virtual bool run() = 0;
  virtual void updateSpeed(float delta) = 0;
  virtual void setSpeed(float speed) = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;
  virtual void setEnablePin(uint8_t pin = 0xff);
};
#endif