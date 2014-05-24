#pragma once

#include "gui/gui_element.h"

template <typename T>
class rect2d;

class gui_slider:public gui_element
{
private:
    float m_min, m_max, m_cur_value, m_old_value, m_slider_pos;
    bool m_vertical;
    glm::vec2 m_mouse_pos;
    void handle_mouse();
    float range(){return m_max-m_min;};
public:
    gui_slider(gui_environment* env, rect2d<int> dimensions, float min, float max, float pos, bool vertical=false);
    virtual ~gui_slider();

    virtual void render();

    bool on_event(const gui_event & e);

    float get_value() const;
    void set_value(float value);
protected:
};
