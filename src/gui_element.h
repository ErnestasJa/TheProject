#pragma once
#include <vector>
#include "rect2d.h"
#include "gui_event_listener.h"

class gui_environment;
class gui_element:public gui_event_listener
{
public:
    gui_element(int x, int y, int w, int h);

    ~gui_element();

    //each element handles it's own rendering
    virtual void render();

    void set_event_listener(gui_event_listener *listener);

    void set_enabled(bool b);

    void set_focused(bool b);

    void set_visible(bool b);

    //!Purpose: Debug
    void set_hovered(bool b);

    void set_listening(bool b);

    bool is_enabled();

    bool is_focused();

    bool is_visible();

    bool is_hovered();

    bool accepts_events();

    gui_environment *get_environment();

    void set_parent(gui_element *e);

    gui_element *get_parent();

    void add_child(gui_element *e);

    void remove_child(gui_element *e);

    void bring_to_front(gui_element *e);

    void render_children();

    gui_element *get_element_from_point(int x, int y);

    void update_absolute_pos();

private:
protected:
    int disp_w, disp_h;
    gui_element *parent;
    std::vector<gui_element*> children;
    gui_environment *environment;
    gui_event_listener *event_listener;
    rect2d<int> *absolute_rect, *relative_rect;
    bool visible, enabled, accept_events, focused, hovered;
};
