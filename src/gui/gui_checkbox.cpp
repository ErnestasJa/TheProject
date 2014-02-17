#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_checkbox.h"

#include "gui_static_text.h"

gui_checkbox::gui_checkbox(gui_environment* env, rect2d<int> dimensions,bool checked):gui_element(env,dimensions)
{
    cur_style=checked?gui_skin_checkbox_c_active:gui_skin_checkbox_u_active;

    environment=env;

    cross="X";

    this->checked=checked;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    this->set_parent(env);
}

gui_checkbox::~gui_checkbox()
{
}

void gui_checkbox::render()
{
    if(enabled&&!hovered)
        cur_style=this->checked?gui_skin_checkbox_c_active:gui_skin_checkbox_u_active;
    else if(!enabled&&!hovered)
        cur_style=this->checked?gui_skin_checkbox_c_disabled:gui_skin_checkbox_u_disabled;

    environment->draw_gui_quad(absolute_rect,cur_style);
}

void gui_checkbox::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case gui_event_type::element_hovered:
        cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_hovered,this));
        break;

    case gui_event_type::element_exitted:
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_exitted,this));
        break;

    case gui_event_type::mouse_pressed:
        cur_style=this->checked?gui_skin_checkbox_c_click:gui_skin_checkbox_u_click;
        break;

    case gui_event_type::mouse_released:
        checked=!checked;
        cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(checkbox_state_changed,this));
        break;
    default:
        break;
    }
}
