#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_button.h"

#include "gui_static_text.h"

#include "gui_skin.h"

gui_button::gui_button(gui_environment* env, rect2d<int> dimensions, std::string text):gui_element(env,dimensions)
{
    environment=env;

    m_text=text;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    cur_style=gui_skin_button_active;

    this->set_parent(env);
}

gui_button::~gui_button()
{
}

void gui_button::render()
{
    if(!enabled)
        cur_col=col_disabled;

    environment->draw_gui_quad(absolute_rect,cur_style);

    glm::vec2 dm=this->environment->get_font_renderer()->get_text_dimensions(this->m_text);
    this->environment->get_font_renderer()->render_string(this->m_text,glm::vec2(this->absolute_rect.x+absolute_rect.w/2-dm.x/2,this->absolute_rect.y+absolute_rect.h/2-dm.y/2),glm::vec4(1),true);
}

void gui_button::set_text(const std::string &text)
{
    this->m_text=text;
}

void gui_button::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case gui_event_type::element_hovered:
        cur_style=gui_skin_button_hover;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_hovered,this));
        break;

    case gui_event_type::element_exitted:
        cur_style=this->enabled?gui_skin_button_active:gui_skin_button_disabled;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_exitted,this));
        break;

    case gui_event_type::mouse_pressed:
        cur_style=gui_skin_button_click;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(button_pressed,this));
        break;

    case gui_event_type::mouse_released:
        cur_style=hovered?gui_skin_button_hover:gui_skin_button_active;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(button_released,this));
        break;
    default:
        break;
    }
}