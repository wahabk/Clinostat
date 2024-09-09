#ifndef LVGL_FLEX_VIEW_H
#define LVGL_FLEX_VIEW_H

#include "LVGLWidget.h"

class LVGLFlexView : public LVGLWidget {
public:
  LVGLFlexView(LVGLWidget* parent)
    : LVGLWidget(parent->getWidget(), parent->getWidget()) {

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_COLUMN);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);
    lv_obj_add_style(parentWidget, &style, 0);
  }
};
#endif