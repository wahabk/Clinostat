#ifndef CONTROL_MOTOR_WIDGET_H
#define CONTROL_MOTOR_WIDGET_H
#include "LVGLWidget.h"
#include "IMotorControl.h"

class MotorControlWidget : public LVGLWidget {
private:
  // stepper itself
  IMotorControl* motorControl;

  // if enambled
  boolean isEnabled = false;

  // label on top
  char* labelText;

  lv_obj_t* topLabel;

  lv_obj_t* speedLabel;

  void createLabel(lv_obj_t* parent, char* labelText) {
    topLabel = lv_label_create(parent);
    speedLabel = lv_label_create(parent);
    lv_label_set_text_fmt(topLabel, "%s", labelText);
    lv_obj_set_pos(topLabel, 5, 5);
    lv_obj_set_pos(speedLabel, 5, 20);
  }

public:
  MotorControlWidget(IMotorControl* motorControl, char* labelText, LVGLWidget* parent)
    : LVGLWidget(parent->getWidget()) {
    this->motorControl = motorControl;
    
    this->labelText = labelText;

    createLabel(widget, labelText);

    // start/stop button
    lv_obj_t* btn = lv_btn_create(widget);
    lv_obj_set_pos(btn, 100, 5);
    lv_obj_add_event_cb(btn, startStopEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, buttonLabel());
    lv_obj_center(label);

    // increase speed
    lv_obj_t* speedButton = lv_btn_create(widget);
    lv_obj_set_pos(speedButton, 160, 5);
    lv_obj_add_event_cb(speedButton, increaseSpeedEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t* speedButtonLabel = lv_label_create(speedButton);
    lv_label_set_text(speedButtonLabel, LV_SYMBOL_UP);
    lv_obj_center(speedButtonLabel);


    // decrease speed
    lv_obj_t* decreaseSpeedButton = lv_btn_create(widget);
    lv_obj_set_pos(decreaseSpeedButton, 220, 5);
    lv_obj_add_event_cb(decreaseSpeedButton, decreaseSpeedEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t* decreaseSpeedButtonLabel = lv_label_create(decreaseSpeedButton);
    lv_label_set_text(decreaseSpeedButtonLabel, LV_SYMBOL_DOWN);
    lv_obj_center(decreaseSpeedButtonLabel);


    lv_obj_set_size(widget, LV_PCT(100), LV_PCT(50));
  }

  char* buttonLabel() {
    if (isEnabled) {
      return LV_SYMBOL_PAUSE;
    } else {
      return LV_SYMBOL_PLAY;
    }
  }

  void updateSpeed(float delta) {
    motorControl->updateSpeed(delta);
  }

  void onEvent(lv_event_t* e) {
    lv_obj_t* btn = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t* label = (lv_obj_t*)lv_obj_get_child(btn, 0);
    isEnabled = !isEnabled;
    if (isEnabled) {
      motorControl->enable();
    } else {
      motorControl->disable();
    }
    lv_label_set_text_fmt(label, buttonLabel());
  }

  static void startStopEventHandler(lv_event_t* e) {
    MotorControlWidget* ctrl = (MotorControlWidget*)lv_event_get_user_data(e);
    ctrl->onEvent(e);
  }

  static void increaseSpeedEventHandler(lv_event_t* e) {
    MotorControlWidget* ctrl = (MotorControlWidget*)lv_event_get_user_data(e);
    ctrl->updateSpeed(1.0);
  }

  static void decreaseSpeedEventHandler(lv_event_t* e) {
    MotorControlWidget* ctrl = (MotorControlWidget*)lv_event_get_user_data(e);
    ctrl->updateSpeed(-1.0);
  }
};

#endif
