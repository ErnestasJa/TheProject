#pragma once
enum gui_event_type
{
    // generic events
    element_focused, element_focus_lost, element_hovered, element_exitted,

    // button events
    button_pressed, button_released,

    // checky boxy
    checkbox_state_changed,

    // scrollbar events
    scrollbar_changed,

    // Window events
    window_dragged, window_closed,

    // GUI Mouse events, pretty much internal
    mouse_pressed, mouse_released, mouse_moved, mouse_dragged,

    // GUI Keyboard events
    key_typed, key_pressed,

    // copy, paste, select
    text_copy, text_paste, text_select,

    // textbox submit
    textbox_submit
};

#define GUI_BEGIN_ON_EVENT(e)  if(this->event_listener)  {if(this->event_listener->OnEvent(e)) return true;}
#define GUI_END_ON_EVENT(e)    if(this->parent)    {if(this->parent->OnEvent(e)) return true;} return false;
#define GUI_FIRE_EVENT(e)  if(this->event_listener) {if(this->event_listener->OnEvent(e)) return true;} if(this->parent){if(this->parent->OnEvent(e)) return true;}
#define GUI_FIRE_ELEMENT_EVENT(el, ev) if((el)->OnEvent((ev))) return;

class GUIElement;
struct GUIEvent
{
public:
    GUIEvent(gui_event_type Type, GUIElement *caller, GUIElement * element)
    {
        this->Type=Type;
        this->caller=caller;
        this->element=element;
    }

    gui_event_type GetType() const
    {
        return this->Type;
    }

    GUIElement *get_caller() const
    {
        return this->caller;
    }

    GUIElement *get_element() const
    {
        return this->element;
    }
private:
    gui_event_type Type;
    GUIElement *caller, *element;
};
