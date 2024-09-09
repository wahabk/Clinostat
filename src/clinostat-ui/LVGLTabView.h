#ifndef LVGL_TAB_VIEW_H
#define LVGL_TAB_VIEW_H

#include "LVGLWidget.h"

// a tab view
class LVGLTabView : public LVGLWidget {
public:
  LVGLTabView(LVGLWidget* parent)
    : LVGLWidget(parent->getWidget()) {

    widget = lv_tabview_create(lv_scr_act(), LV_DIR_LEFT, 80);

    lv_obj_set_style_bg_color(widget, lv_palette_lighten(LV_PALETTE_RED, 2), 0);

    lv_obj_t* tab_btns = lv_tabview_get_tab_btns(widget);
    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_clear_flag(lv_tabview_get_content(widget), LV_OBJ_FLAG_SCROLLABLE);
  }

  LVGLWidget* addTab(const char* name) {
    return new LVGLWidget(this->getWidget(), lv_tabview_add_tab(widget, name));
  }
};

#endif