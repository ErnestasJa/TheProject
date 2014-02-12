#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_button.h"

#include "gui_static_text.h"

gui_button::gui_button(gui_environment* env, rect2d<int> dimensions, std::string text):gui_element(env,dimensions)
{
    col_active=helpers::color255(30,40,140);
    col_disabled=helpers::color255(0,10,110);
    col_hover=helpers::color255(50,60,160);
    col_clicked=helpers::color255(40,50,150);
    col_text=helpers::color255();
    cur_col=col_active;

    environment=env;

    m_text=text;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_background=new quad();
    m_background->generate();

    _material=env->get_quad_shader();

    this->set_parent(env);
}

gui_button::~gui_button()
{
    destroy_children();
}

void gui_button::render()
{
    this->m_transform=glm::mat4(1.0f);

    float gsx=environment->get_gui_scale().x;
    float gsy=environment->get_gui_scale().y;
    float px=-1+absolute_rect.x*gsx+absolute_rect.w/2*gsx;
    float py=1-absolute_rect.y*gsy-absolute_rect.h/2*gsy;
    float sx=absolute_rect.w/2*gsx;
    float sy=absolute_rect.h/2*gsy;

    this->m_transform=glm::translate(this->m_transform,glm::vec3(px,py,0));
    this->m_transform=glm::scale(this->m_transform,glm::vec3(sx,sy,0));

    _material->set();
    glUniformMatrix4fv(_material->getparam("M"),1,GL_FALSE,glm::value_ptr(m_transform));
    glUniform3fv(_material->getparam("C"),1,glm::value_ptr(cur_col));
    glBindTexture(GL_TEXTURE_2D,0);
    this->m_background->draw();

    glm::vec2 dm=this->environment->get_font_renderer()->get_text_dimensions(this->m_text);
    this->environment->get_font_renderer()->render_string(this->m_text,glm::vec2(this->absolute_rect.x+absolute_rect.w/2-dm.x/2,this->absolute_rect.y+absolute_rect.h/2-dm.y/2),this->col_text);
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
        cur_col=col_hover;
        break;
    case gui_event_type::element_exitted:
        cur_col=this->enabled?col_active:col_disabled;
        break;
    case gui_event_type::mouse_pressed:
        cur_col=col_clicked;
        this->event_listener->on_event(gui_event(button_pressed,this));
        break;
    case gui_event_type::mouse_released:
        cur_col=hovered?col_hover:col_active;
        this->event_listener->on_event(gui_event(button_released,this));
        break;
    default:
        break;
    }
}
