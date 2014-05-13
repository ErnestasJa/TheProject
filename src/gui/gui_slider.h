#pragma once

#include "gui/gui_element.h"

template <typename T>
class rect2d;

class gui_slider:public gui_element
{
private:
    int m_min, m_max, m_cur_value, m_old_value, m_slider_pos;
    bool m_vertical;
    glm::vec2 m_mouse_pos;
    void handle_mouse();
public:
    gui_slider(gui_environment* env, rect2d<int> dimensions, int min, int max, int pos, bool vertical=false);
    virtual ~gui_slider();

    virtual void render();

    bool on_event(const gui_event & e);

    int get_value() const;
    void set_value(int value);
protected:
};
