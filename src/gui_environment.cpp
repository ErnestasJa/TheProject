#include "precomp.h"
#include "gui_environment.h"
#include "input_handler.h"

gui_environment::gui_environment(int dispw, int disph):gui_element(0,0,dispw,disph)
{
    this->input=new input_handler(nullptr);
    hover=last_hover=focus=last_focus=nullptr;
}

gui_environment::~gui_environment()
{

}

void gui_environment::update(float delta)
{
    //hovering
    vec2<int> tm=input->get_mouse_pos();

    gui_element *target = get_element_from_point(tm.x, tm.y);

    //only update elements which are enabled,visible and accept events
    if (target != nullptr)
        if (target->is_enabled() && target->is_visible() && target->accepts_events())
        {
            mouse_pos = vec2<int>(tm.x, tm.y);

            if (target != hover && target != nullptr && mouse_dragged == false)
            {
                if (hover != nullptr)
                {
                    last_hover = hover;
                    last_hover->on_event(gui_event(
                                             gui_event_type::element_exitted,
                                             last_hover));
                }
                hover = target;
                hover->on_event(gui_event(
                                    gui_event_type::element_hovered, hover));
            }

            //focusing
            if (input->mouse_button(0) && hover != focus
                    && mouse_dragged == false)
            {
                if (focus != nullptr)
                {
                    last_focus = focus;
                    last_focus->on_event(gui_event(
                                             gui_event_type::element_focus_lost,
                                             last_focus));
                }
                focus = hover;
                if (hover != this)
                {
                    focus->on_event(gui_event(
                                        gui_event_type::element_focused, focus));
                    //if(focus->get_parent())
                    focus->get_parent()->bring_to_front(focus);
                }
                else
                    focus = nullptr;
            }

            if (input->mouse_button(0) && focus != nullptr
                    && mouse_down == false)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_pressed, this));
                mouse_down = true;
            }

            if (!input->mouse_button(0) && mouse_down && focus != nullptr)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_released, this));
                mouse_down = false;
            }

            //TODO: keyboard typing
//            if(focus!=null&&Keyboard.next()&&Keyboard.getEventCharacter()!=Keyboard.CHAR_NONE)
//            {
//                focus.onEvent(new GUIEvent(GUIEvent.GUIEventType.KeyTyped, this));
//            }

            if (mouse_pos != last_mouse_pos)
            {
                mouse_moved = true;
                if (input->mouse_button(0))
                    mouse_dragged = true;
                else
                    mouse_dragged = false;
            }
            else
                mouse_moved = false;

            if (mouse_moved && focus != nullptr && !mouse_dragged)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_moved, this));

            if (mouse_down && mouse_moved && focus != nullptr)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_dragged, this));
        }
}

void gui_environment::render()
{

}

void gui_environment::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case element_focused:
        this->bring_to_front(e.get_caller());
        break;
    default:
        break;
    }
}
