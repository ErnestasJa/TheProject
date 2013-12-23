#pragma once
#include "gui_element.h"
#include "vec2.h"
// Stuff to still convert
//!TODO (murloc992#1#): GUI Event
//!TODO (murloc992#1#): GUI Event Listener
//!TODO (murloc992#1#): Rectangle
//!TODO (murloc992#1#): Vec2
//!TODO (murloc992#1#): GUI Element
//!TODO (murloc992#1#): GUI Environment
//TODO (murloc992#1#): GUI Button
//TODO (murloc992#1#): GUI Window
//TODO (murloc992#1#): GUI Static Text
//TODO (murloc992#1#): GUI Text Field
//TODO (murloc992#1#): GUI Slider

class input_handler;

class gui_environment : public gui_element
{
public:
    gui_environment(int dispw, int disph, GLFWwindow* win);
    ~gui_environment();
    void update(float delta);
    void render();

    void on_event(gui_event e);
    bool is_on_hover(gui_element *e);
    bool is_on_focus(gui_element *e);
private:
    input_handler *input;
    GLFWwindow* window;
    gui_element *hover, *last_hover, *focus, *last_focus;
    bool m_mouse_down, m_mouse_moved, m_mouse_dragged;
    vec2<double> mouse_pos, last_mouse_pos;
protected:
};
