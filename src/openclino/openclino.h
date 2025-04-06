// openclino.h
#ifndef OPENCLINO_H
#define OPENCLINO_H

#include <Arduino.h>

// Pin definitions
extern const int buttonPin;
extern const int enablePinX;
extern const int stepPinX;
extern const int dirPinX;
extern const int enablePinY;
extern const int stepPinY;
extern const int dirPinY;

// Motor and gear ratio constants
extern const int motorSteps;
extern const int subStep;
extern const unsigned long stepsPerRevolution;
extern const double nStepsPerRotX;
extern const double nStepsPerRotY;

// Function declarations
void enable(bool enableX, bool enableY);
void spin_degs(float Q1_target, float Q2_target, float maxRPM = 10, int finalDelay = 100);
void spin_continuous(float speedX = 10, float speedY = 10, int buttonDelay = 1000);
void calibrate_y_correction();
void test_pulley_ratios();
void test_spin_degs_multi();

#endif