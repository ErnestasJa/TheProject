#pragma once
enum gui_event_type
{
    // generic events
    element_focused, element_focus_lost, element_hovered, element_exitted,

    // button events
    button_pressed, button_released,

    // scrollbar events
    scrollbar_changed,

    // Window events
    window_dragged, window_closed,

    // GUI Mouse events, pretty much internal
    mouse_pressed, mouse_released, mouse_moved, mouse_dragged,

    // GUI Keyboard events
    key_typed
};

class gui_element;
struct gui_event
{
public:
    gui_event(gui_event_type type,gui_element *caller)
    {
        this->type=type;
        this->caller=caller;
    }

    gui_event_type get_type()
    {
        return this->type;
    }

    gui_element *get_caller()
    {
        return this->caller;
    }
private:
    gui_event_type type;
    gui_element *caller;
};
