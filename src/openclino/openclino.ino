/*
 * OpenClino
 * 
 * X is the outer frame.
 * Y is the inner frame.
 */

#include "openclino.h"
#include <SPI.h>
#include <SD.h>

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(enablePinX, OUTPUT);
  pinMode(enablePinY, OUTPUT);
  Serial.begin(9600);
  Serial.print("--\nHello from OpenClino.\n--\n");
  // print gear ratios and nStepsPerRotX, nStepsPerRotY
  Serial.print("xRatio: ");
  Serial.println(xRatio);
  Serial.print("yRatio: ");
  Serial.println(yRatio);
  Serial.print("nStepsPerRotX: ");
  Serial.println(nStepsPerRotX);
  Serial.print("nStepsPerRotY: ");
  Serial.println(nStepsPerRotY);
  Serial.print("--\n");

  enable(false, false);
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  // Turn off motors when button is not pressed.
  enable(false, false);

  if (buttonState == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    enable(true, true);
    // Uncomment to run calibration
    calibrate_y_correction();
    test_pulley_ratios();
    test_spin_degs_multi();
    // Uncomment to run continuous spin
    // spin_continuous(10, 10);
  }
}
