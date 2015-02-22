#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;

class GUISlider:public GUIElement
{
private:
    float m_min, m_max, m_cur_value, m_old_value, m_slider_pos;
    bool m_vertical;
    glm::vec2 m_mouse_pos;
    void handle_mouse();
    float range(){return m_max-m_min;};
public:
    GUISlider(GUIEnvironment* env, Rect2D<int> dimensions, float min, float max, float pos, bool vertical=false);
    virtual ~GUISlider();

    virtual void Render();

    bool OnEvent(const GUIEvent & e);

    float get_value() const;
    void set_value(float value);
protected:
};
