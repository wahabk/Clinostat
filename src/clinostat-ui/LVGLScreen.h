#ifndef LVGL_SCREEN_H
#define LVGL_SCREEN_H

#include "LVGLWidget.h"
#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"

// the whole screen
class LVGLScreen : public LVGLWidget {
public:
  LVGLScreen(Arduino_H7_Video* display)
    : LVGLWidget(lv_scr_act()) {
    lv_obj_set_size(widget, display->width(), display->height());
  }
};
#endif
