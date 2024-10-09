#ifndef LVGL_WIDGET_H
#define LVGL_WIDGET_H

#include "lvgl.h"

// a generic widget
class LVGLWidget {

protected:
  lv_obj_t* widget;
  lv_obj_t* parentWidget;

public:
  LVGLWidget(lv_obj_t* parentWidget) {
    this->parentWidget = parentWidget;
    this->widget = lv_obj_create(parentWidget);
  }

  LVGLWidget(lv_obj_t* parentWidget, lv_obj_t* widget) {
    this->parentWidget = parentWidget;
    this->widget = widget;
  }


  lv_obj_t* getWidget() {
    return this->widget;
  }
};

#endif
