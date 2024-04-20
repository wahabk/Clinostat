//#include "core/lv_disp.h"
#ifndef UI_H
#define UI_H

#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "LVGLScreen.h"
#include "LVGLTabView.h"
#include "LVGLFlexView.h"
#include "IMotorControl.h"
#include "MotorControlWidget.h"
#include "Runnable.h"
#include "ClinostatControl.h"
#include "ClinostatControlWidget.h"

class UI : public Runnable {
  Arduino_H7_Video* display;
  Arduino_GigaDisplayTouch* touchDetector;
  Motors* motors;
  ClinostatControl* clino;
public:
  UI(Arduino_H7_Video* display, Arduino_GigaDisplayTouch* touchDetector, Motors* motors) {
    this->display = display;
    this->touchDetector = touchDetector;
    this->motors = motors;
    this->clino = new ClinostatControl(motors);
  }

  void setup() {
    display->begin();
    touchDetector->begin();
    LVGLScreen* screen = new LVGLScreen(display);
    LVGLTabView* tabView = new LVGLTabView(screen);
    
    LVGLWidget* clinostatTab = tabView->addTab("Clinostat");
    //LVGLWidget* clinostatTabWindow = new LVGLFlexView(clinostatTab);
    new ClinostatControlWidget(clino, clinostatTab);

    // setup motor tab
    LVGLWidget* controlMotorTab = tabView->addTab("Control motor");
    LVGLWidget* controlMotorTabWindow = new LVGLFlexView(controlMotorTab);
    new MotorControlWidget(motors->get(X_MOTOR), "X Motor", controlMotorTabWindow);
    new MotorControlWidget(motors->get(Y_MOTOR), "Y Motor", controlMotorTabWindow);
  }

  void loop() {
    lv_timer_handler();
  }
};

#endif
