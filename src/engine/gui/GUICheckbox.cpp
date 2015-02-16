#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUICheckbox.h"

#include "GUIStaticText.h"

GUICheckbox::GUICheckbox(GUIEnvironment* env, Rect2D<int> dimensions,bool checked):GUIElement(env,dimensions)
{
    this->Type=GUIET_CHECKBOX;
    cur_style=checked?gui_skin_checkbox_c_active:gui_skin_checkbox_u_active;

    environment=env;

    this->checked=checked;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    this->SetParent(env);
}

GUICheckbox::~GUICheckbox()
{
}

void GUICheckbox::Render()
{
    if(enabled&&!hovered)
        cur_style=this->checked?gui_skin_checkbox_c_active:gui_skin_checkbox_u_active;
    else if(!enabled&&!hovered)
        cur_style=this->checked?gui_skin_checkbox_c_disabled:gui_skin_checkbox_u_disabled;

    environment->draw_gui_quad(absolute_rect,cur_style);
}

bool GUICheckbox::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

        switch(e.GetType())
        {
        case gui_event_type::element_hovered:
            cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
            GUI_FIRE_EVENT(GUIEvent(element_hovered,this,this))
            break;

        case gui_event_type::element_exitted:
            GUI_FIRE_EVENT(GUIEvent(element_exitted,this,this));
            break;

        case gui_event_type::mouse_pressed:
            cur_style=this->checked?gui_skin_checkbox_c_click:gui_skin_checkbox_u_click;
            break;

        case gui_event_type::mouse_released:
            checked=!checked;
            cur_style=this->checked?gui_skin_checkbox_c_hover:gui_skin_checkbox_u_hover;
            GUI_FIRE_EVENT(GUIEvent(checkbox_state_changed,this,this))
            break;
        default:
            break;
        }

    GUI_END_ON_EVENT(e)
}
