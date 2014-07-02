#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUICheckbox.h"

#include "GUIStaticText.h"

gui_checkbox::gui_checkbox(gui_environment* env, rect2d<int> dimensions,bool checked):gui_element(env,dimensions)
{
    this->type=GUIET_checkbox;
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

bool gui_checkbox::on_event(const gui_event & e)
{
    GUI_BEGIN_ON_EVENT(e)

        switch(e.get_type())
        {
        case gui_event_type::element_hovered:
            cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
            GUI_FIRE_EVENT(gui_event(element_hovered,this,this))
            break;

        case gui_event_type::element_exitted:
            GUI_FIRE_EVENT(gui_event(element_exitted,this,this));
            break;

        case gui_event_type::mouse_pressed:
            cur_style=this->checked?gui_skin_checkbox_c_click:gui_skin_checkbox_u_click;
            break;

        case gui_event_type::mouse_released:
            checked=!checked;
            cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
            GUI_FIRE_EVENT(gui_event(checkbox_state_changed,this,this))
            break;
        default:
            break;
        }

    GUI_END_ON_EVENT(e)
}
