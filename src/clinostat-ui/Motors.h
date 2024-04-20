#ifndef MOTORS_H
#define MOTORS_H

#include "Runnable.h"

class Motors : public Runnable {
public:
  virtual IMotorControl* get(int idx) = 0;
};
#endif