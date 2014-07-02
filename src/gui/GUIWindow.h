#pragma once

#include "gui/gui_element.h"

template <typename T>
class rect2d;

class gui_button;
class gui_window:public gui_element
{
private:
    bool clip,showclose,modal,dragging,movable;
    std::wstring titlebar_text;

    glm::mat4 m_transform;
    gui_button* close_btn;


    glm::vec2 mp,dif,ds;
    void move(glm::vec2 delta);

    rect2d<int> tbr,bgr;
public:
    gui_window(gui_environment* env, rect2d<int> dimensions, std::wstring titlebar_text=L"Window", bool clip=true, bool showclose=true, bool modal=false, bool movable=true);
    virtual ~gui_window();

    void render();

    virtual bool on_event(const gui_event & e);
protected:
};
