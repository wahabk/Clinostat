#define LV_SPRINTF_DISABLE_FLOAT 0
#include <RPC.h>
#include "LocalMotors.h"
#include "RemoteMotors.h"
#include "UI.h"
#include "ClinostatControl.h"

// when false we run both UI and motors on the same core.
boolean multiCore = true;

Arduino_H7_Video* Display;
Arduino_GigaDisplayTouch* TouchDetector;

Motors* motors;
UI* ui;

// flags controlling what to run
boolean runUi;
boolean runMotors;

boolean isCm4Core() {
  return HAL_GetCurrentCPUID() == CM4_CPUID;
}


void setup() {
  Serial.begin(9600);
  if (multiCore) {
    RPC.begin();
  }

  // check who runs what
  runUi = multiCore == false || HAL_GetCurrentCPUID() == CM7_CPUID;
  runMotors = multiCore == false || isCm4Core();
  
  if (runMotors) {
    motors = new LocalMotors(multiCore == true && isCm4Core());
    motors->setup();
  } else {
    motors = new RemoteMotors();
    motors->setup();
  }

  if (runUi) {
   Display = new Arduino_H7_Video(800, 480, GigaDisplayShield);
   TouchDetector = new Arduino_GigaDisplayTouch();
   ui = new UI(Display, TouchDetector, motors);
   ui->setup();
  }
  
  ClinostatControl* ctrl = new ClinostatControl(motors);
  ctrl->setSpeed(7.2, 16);
  //ctrl->start();
}

void loop() {
  if (runMotors) {
    motors->loop();
  }

  if (runUi) {
     ui->loop();
  }
}