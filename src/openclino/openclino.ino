/*
 * OpenClino
 * 
 * X is the outer frame.
 * Y is the inner frame.
 */
#include <Arduino.h>
#include "openclino.h"
#include <SPI.h>
#include <SD.h>

void setup()
{
  setupPins();
  setupSerial();
  enable(false, false);
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  // Disable motors when button is not pressed
  enable(false, false);

  if (buttonState == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    enable(true, true);
    // Uncomment to run calibration
    // calibrate_y_correction();
    test_pulley_ratios();
    test_spin_degs_multi();
    // Uncomment to run continuous spin at 10 rpm for both axes
    // spin_continuous(10, 10);
    // Uncomment to run follow_path
    // follow_path();
  }
}
