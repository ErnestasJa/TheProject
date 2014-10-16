#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUIButton.h"

#include "GUIStaticText.h"

#include "GUISkin.h"

#include "GUIImage.h"

gui_button::gui_button(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text,bool toggle,bool toggle_status):GUIElement(env,dimensions)
{
    this->Type=GUIET_BUTTON;
    environment=env;

    m_text=text;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    cur_style=gui_skin_button_active;

    m_toggled=toggle_status;
    m_toggle=toggle;

    m_overlay_image=nullptr;
    this->SetParent(env);
}

gui_button::~gui_button()
{
}

void gui_button::Render()
{
    if(!enabled)
        cur_col=col_disabled;

    environment->draw_sliced_gui_quad(absolute_rect,m_toggled&&m_toggle?gui_skin_button_click:enabled?cur_style:gui_skin_button_disabled);

    glm::vec2 dm=this->environment->get_font_renderer()->GetTextDimensions(this->m_text);
    this->environment->get_font_renderer()->RenderString(L"['s]"+this->m_text+L"[s']",glm::vec2(this->absolute_rect.x+absolute_rect.w/2-dm.x/2,this->absolute_rect.y+((this->absolute_rect.h-dm.y)/2)));

    this->RenderChildren();
}

void gui_button::set_text(const std::wstring &text)
{
    this->m_text=text;
}

void gui_button::set_image(gui_image* img)
{
    if(m_overlay_image!=nullptr)
    {
        remove_image();
    }
    else
    {
        this->m_overlay_image=img;
        this->AddChild(img);
    }
}

void gui_button::remove_image()
{
    this->RemoveChild(m_overlay_image);
    m_overlay_image=nullptr;
}

bool gui_button::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)
    switch(e.GetType())
    {
    case gui_event_type::element_hovered:
        cur_style=gui_skin_button_hover;
            GUI_FIRE_EVENT(GUIEvent(element_hovered,this,this))
        break;

    case gui_event_type::element_exitted:
        cur_style=this->enabled?gui_skin_button_active:gui_skin_button_disabled;
            GUI_FIRE_EVENT(GUIEvent(element_exitted,this,this))
        break;

    case gui_event_type::mouse_pressed:
        cur_style=gui_skin_button_click;
            GUI_FIRE_EVENT(GUIEvent(button_pressed,this,this))
        break;

    case gui_event_type::mouse_released:
        if(m_toggle)
            m_toggled=!m_toggled;
        cur_style=hovered?gui_skin_button_hover:gui_skin_button_active;
            GUI_FIRE_EVENT(GUIEvent(button_released,this,this))
        break;

    default:
        break;
    }
    GUI_END_ON_EVENT(e)
}
