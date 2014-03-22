#pragma once

#include "gui/gui_element.h"

template <typename T>
class rect2d;

class gui_slider:public gui_element
{
private:
    int m_min, m_max, m_cur_value, m_old_value, m_slider_pos;
    glm::vec2 m_mouse_pos;
public:
    gui_slider(gui_environment* env, rect2d<int> dimensions, int min, int max, int pos);
    virtual ~gui_slider();

    virtual void render();

    void on_event(gui_event e);
protected:
};
