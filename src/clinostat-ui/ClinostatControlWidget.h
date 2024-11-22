#include "extra/widgets/spinbox/lv_spinbox.h"

#ifndef CLINOSTAT_CONTROL_WIDGET_H
#define CLINOSTAT_CONTROL_WIDGET_H
#include "LVGLWidget.h"
#include "ClinostatControl.h"
#include "core/lv_obj_style.h"
#include "font/lv_font.h"
#include "misc/lv_style.h"

// allows controlling speed of clinostat axes
class ClinostatControlWidget : public LVGLWidget {
private:
  ClinostatControl* control;

  lv_obj_t* xSpeedSlider;

  lv_obj_t* ySpeedSlider;

  lv_obj_t* startStopButton;

  volatile boolean running = false;

  char* buttonLabel() {
    if (running) {
      return LV_SYMBOL_PAUSE;
    } else if (control->canRun()) {
      return LV_SYMBOL_PLAY;
    } else {
      return "Set speed please";
    }
  }

  void updateStartStopButtonLabel() {
    lv_obj_t* label = lv_obj_get_child(startStopButton, 0);
    lv_label_set_text_fmt(label, buttonLabel());
  }

public:
  ClinostatControlWidget(ClinostatControl* control, LVGLWidget* parent)
    : LVGLWidget(parent->getWidget(), parent->getWidget()) {
    this->control = control;

    // set style so we can reda the values
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_28);
    lv_obj_add_style(widget, &style, 0);


    // create x/y spins    
    xSpeedSlider = createSpinner(30, 5, (int)((double)control->getXSpeed() * 10.0));
    ySpeedSlider = createSpinner(30, 80, (int)((double)control->getYSpeed() * 10.0));


    // start/stop button
    startStopButton = lv_btn_create(widget);
    lv_obj_set_pos(startStopButton, 400, 80);
    lv_obj_add_event_cb(startStopButton, startStopEventHandler, LV_EVENT_CLICKED, this);
    lv_obj_t* label = lv_label_create(startStopButton);
    lv_label_set_text(label, buttonLabel());
    lv_obj_center(label);
  }

  lv_obj_t* createSpinner(int startX, int startY, int initialSpeed) {
    lv_obj_t* xSpeedSlider = lv_spinbox_create(widget);

    lv_obj_set_pos(xSpeedSlider, startX + 60, startY);
    lv_spinbox_set_range(xSpeedSlider, -300, 300);
    lv_spinbox_set_digit_format(xSpeedSlider, 3, 2);
    lv_spinbox_step_prev(xSpeedSlider);
    lv_spinbox_set_value(xSpeedSlider, initialSpeed);
    lv_obj_add_event_cb(xSpeedSlider, xSpeedSliderEventCb, LV_EVENT_VALUE_CHANGED, this);
    lv_coord_t h = lv_obj_get_height(xSpeedSlider);

    lv_obj_t* btn = lv_btn_create(widget);
    lv_obj_set_size(btn, h, h);
    lv_obj_set_pos(btn, startX + 200, startY);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL, xSpeedSlider);

    btn = lv_btn_create(widget);
    lv_obj_set_size(btn, h, h);
    lv_obj_set_pos(btn, startX, startY);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, xSpeedSlider);

    return xSpeedSlider;
  }

  void updateSpeed() {
    control->setSpeed(((float)lv_spinbox_get_value(xSpeedSlider))/10.0, ((float)lv_spinbox_get_value(ySpeedSlider))/10.0);
    updateStartStopButtonLabel();
  }

  void toggleRunning() {
    if (control->canRun()) {
      running = !running;
      updateStartStopButtonLabel();
      if (running) {
        control->start();
      } else {
        control->stop();
      }
    }
  }


  static void lv_spinbox_increment_event_cb(lv_event_t* e) {
    lv_obj_t* spinbox = (lv_obj_t*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
      lv_spinbox_increment(spinbox);
    }
  }

  static void lv_spinbox_decrement_event_cb(lv_event_t* e) {
    lv_obj_t* spinbox = (lv_obj_t*)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
      lv_spinbox_decrement(spinbox);
    }
  }

  static void startStopEventHandler(lv_event_t* e) {
    ClinostatControlWidget* ctrl = (ClinostatControlWidget*)lv_event_get_user_data(e);
    ctrl->toggleRunning();
  }


  static void xSpeedSliderEventCb(lv_event_t* e) {
    ClinostatControlWidget* ctrl = (ClinostatControlWidget*)lv_event_get_user_data(e);
    ctrl->updateSpeed();
  }
};

#endif
