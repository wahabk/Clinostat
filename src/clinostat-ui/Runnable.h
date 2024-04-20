#ifndef RUNNABLE_H
#define RUNNABLE_H
class Runnable {
public:
  virtual void setup() = 0;
  virtual void loop();
};
#endif