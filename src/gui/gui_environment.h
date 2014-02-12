#pragma once

// Stuff to still convert
//!TODO (murloc992#1#): GUI Event
//!TODO (murloc992#1#): GUI Event Listener
//!TODO (murloc992#1#): Rectangle
//!TODO (murloc992#1#): Vec2
//!TODO (murloc992#1#): GUI Element
//!TODO (murloc992#1#): GUI Environment
//!TODO (murloc992#1#): Font Renderer
//TODO (murloc992#1#): GUI Static Text
//TODO (murloc992#1#): GUI Button
//TODO (murloc992#1#): GUI Window
//TODO (murloc992#1#): GUI Text Field
//TODO (murloc992#1#): GUI Slider

#include "gui_element.h"
#include "input_handler.h"
#include "gui_event.h"
#include "gui_event_listener.h"

#include "font_renderer.h"

class shader;
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

    shader* get_quad_shader()
    {
        return this->gui_shader;
    }
    void draw_gui_quad(rect2d<int> size);

    glm::vec2 get_mouse_pos();
    glm::vec2 get_gui_scale();

    template <typename T>
    rect2d<T> scale_gui_rect(rect2d<T> unscaled)
    {
        T gsx=get_gui_scale().x;
        T gsy=get_gui_scale().y;
        T px=-1+unscaled.x*gsx+unscaled.w/2*gsx;
        T py=1-unscaled.y*gsy-unscaled.h/2*gsy;
        T sx=unscaled.w/2*gsx;
        T sy=unscaled.h/2*gsy;

        rect2d<T> ret=rect2d<T>(px,py,sx,sy);

        return ret;
    }

    font_renderer *get_font_renderer();
private:
    shader* gui_shader;
    font_renderer* m_font_renderer;
    input_handler *input;
    GLFWwindow* window;
    gui_element *hover, *last_hover, *focus, *last_focus;
    bool m_mouse_down, m_mouse_moved, m_mouse_dragged;
    glm::vec2 mouse_pos, last_mouse_pos, gui_scale;
protected:
};
