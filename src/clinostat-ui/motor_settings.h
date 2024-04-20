#ifndef MOTOR_SETTINGS_H
#define MOTOR_SETTINGS_H

// Your motor settings

// should match your motor specs
const int motorSteps = 200;
// should match your controller
const int subStep = 32;
const int stepsPerRevolution = motorSteps * subStep;

// should match clinostate build

// dimater of X motor pulley
const float xMotD = 12.0;
const float xPulleyD = 31.5;
const float yMotD = 12.0;
const float yTurnD = 31.5;
const float yPulleyD = 18.5;
const float xRatio = xPulleyD/xMotD; // diameters
const float yRatio = yPulleyD/yMotD;

const int X_MOTOR = 0;
const int Y_MOTOR = 1;
const int NO_MOTORS = 2;

#endif