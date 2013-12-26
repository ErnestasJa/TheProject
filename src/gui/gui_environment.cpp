#include "precomp.h"

#include "gui_environment.h"
#include "input_handler.h"

gui_environment::gui_environment(int dispw, int disph, GLFWwindow* win):gui_element(0,0,dispw,disph)
{
    this->window=win;
    this->input=new input_handler(nullptr,window);
    hover=last_hover=focus=last_focus=nullptr;
    m_mouse_down=m_mouse_dragged=m_mouse_moved=false;
    mouse_pos=last_mouse_pos=glm::vec2();
    gui_scale=glm::vec2(2.0/(float)dispw,2.0/(float)disph);
    this->set_name("GUI_ENVIRONMENT");
}

gui_environment::~gui_environment()
{

}

void gui_environment::update(float delta)
{
    //hovering
    glm::vec2 tm=input->get_mouse_pos();
    mouse_pos=input->get_mouse_pos();
    //printf("tm: %i %i c:%i %i old:%i %i\n",tm.x,tm.y,mouse_pos.x,mouse_pos.y,last_mouse_pos.x,last_mouse_pos.y);

    gui_element *target = get_element_from_point(tm.x, tm.y);

    //only update elements which are enabled,visible and accept events
    if (target != nullptr)
        if (target->is_enabled() && target->is_visible() && target->accepts_events())
        {

            if (target != hover && target != nullptr && m_mouse_dragged == false)
            {
                if (hover != nullptr)
                {
                    last_hover = hover;
                    last_hover->on_event(gui_event(
                                             gui_event_type::element_exitted,
                                             last_hover));
                                             last_hover->set_hovered(false);
                }
                hover = target;
                hover->on_event(gui_event(
                                    gui_event_type::element_hovered, hover));
                                    hover->set_hovered(true);
            }

            //focusing
            if (input->mouse_button(0) && hover != focus
                    && m_mouse_dragged == false)
            {
                if (focus != nullptr)
                {
                    last_focus = focus;
                    last_focus->on_event(gui_event(
                                             gui_event_type::element_focus_lost,
                                             last_focus));
                                             last_focus->set_focused(false);
                }
                focus = hover;
                if (hover != this)
                {
                    focus->on_event(gui_event(
                                        gui_event_type::element_focused, focus));
                                        focus->set_focused(true);
                    focus->get_parent()->bring_to_front(focus);
                }
                else
                    focus = nullptr;
            }

            if (input->mouse_button(0) && focus != nullptr
                    && m_mouse_down == false)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_pressed, this));
                m_mouse_down = true;
            }

            if (!input->mouse_button(0) && m_mouse_down && focus != nullptr)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_released, this));
                m_mouse_down = false;
            }

            //TODO: keyboard typing
//            if(focus!=null&&Keyboard.next()&&Keyboard.getEventCharacter()!=Keyboard.CHAR_NONE)
//            {
//                focus.onEvent(new GUIEvent(GUIEvent.GUIEventType.KeyTyped, this));
//            }

            if (mouse_pos != last_mouse_pos)
            {
                m_mouse_moved = true;
                last_mouse_pos=mouse_pos;
                //printf("mouse has moved..\n");
                if (input->mouse_button(0))
                    m_mouse_dragged = true;
                else
                    m_mouse_dragged = false;
            }
            else
                m_mouse_moved = false;

            if (m_mouse_moved && focus != nullptr && !mouse_dragged)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_moved, this));

            if (m_mouse_down && m_mouse_moved && focus != nullptr)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_dragged, this));
        }
}

void gui_environment::render()
{
    this->render_children();
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

glm::vec2 gui_environment::get_mouse_pos()
{
    return this->input->get_mouse_pos();
}

glm::vec2 gui_environment::get_gui_scale()
{
    return gui_scale;
}
