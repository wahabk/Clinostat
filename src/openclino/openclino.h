/*
 * OpenClino
 * 
 * X is the outer frame.
 * Y is the inner frame.
 */
#ifndef OPENCLINO_H
#define OPENCLINO_H

#include <Arduino.h>
#include <SD.h>

// 🛠 Motor & Button Pins
extern const int buttonPin;
extern const int enablePinX;
extern const int stepPinX;
extern const int dirPinX;
extern const int enablePinY;
extern const int stepPinY;
extern const int dirPinY;
extern int buttonState;

// Define steps per revolution
extern const int motorSteps;
extern const int subStep;
extern const unsigned long stepsPerRevolution;

// Pulley teeth
extern const float xMotTeeth;
extern const float xPulleyTeeth;
extern const float yMotTeeth;
extern const float yTurnTeeth;
extern const float yPulleyTeeth;

// Calculate the gear ratios based on pulley teeth and calibration factor
extern const double xRatio;
extern const double yRatio1;
extern const double yRatio2;
extern double yCorrection;
extern double yRatio;

// Calculate effective steps per rotation
extern const double nStepsPerRotX;
extern double nStepsPerRotY;

// SD definitions
extern File pathFile;
extern const char* PATH_FILENAME;
const int chipSelect = 10;

// Function declarations
void setupPins();
void setupSerial();
void enable(bool enableX, bool enableY);
void spin_degs(float Q1_target, float Q2_target, float maxRPM = 10, int finalDelay = 100);
void spin_continuous(float speedX = 10, float speedY = 10, int buttonDelay = 1000);
void calibrate_y_correction();
void test_pulley_ratios();
void test_spin_degs_multi();
void follow_path();

#endif
