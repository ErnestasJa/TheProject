#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_checkbox.h"

#include "gui_static_text.h"

gui_checkbox::gui_checkbox(gui_environment* env, rect2d<int> dimensions,bool checked):gui_element(env,dimensions)
{
    col_active=helpers::color255(30,40,140);
    col_disabled=helpers::color255(0,10,110);
    col_hover=helpers::color255(50,60,160);
    col_clicked=helpers::color255(40,50,150);
    col_text=helpers::color255();
    cur_col=col_active;

    environment=env;

    cross="X";

    this->checked=checked;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_background=new quad();
    m_background->generate();

    _material=env->get_quad_shader();

    this->set_parent(env);
}

gui_checkbox::~gui_checkbox()
{
    destroy_children();
}

void gui_checkbox::render()
{
    if(!enabled)
        cur_col=col_disabled;
    this->m_transform=glm::mat4(1.0f);

    rect2d<float> scaled=environment->scale_gui_rect<float>(absolute_rect.as<float>());

    this->m_transform=glm::translate(this->m_transform,glm::vec3(scaled.x,scaled.y,0));
    this->m_transform=glm::scale(this->m_transform,glm::vec3(scaled.w,scaled.h,0));

    _material->set();
    glUniformMatrix4fv(_material->getparam("M"),1,GL_FALSE,glm::value_ptr(m_transform));
    glUniform3fv(_material->getparam("C"),1,glm::value_ptr(cur_col));
    glBindTexture(GL_TEXTURE_2D,0);
    this->m_background->draw();
    if(this->checked)
    {
        glm::vec2 dm=this->environment->get_font_renderer()->get_text_dimensions(this->cross);
        this->environment->get_font_renderer()->render_string(this->cross,glm::vec2(this->absolute_rect.x+absolute_rect.w/2-dm.x/2,this->absolute_rect.y+absolute_rect.h/2-dm.y/2),this->col_text,true);
    }
}

void gui_checkbox::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case gui_event_type::element_hovered:
        cur_col=col_hover;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_hovered,this));
        break;

    case gui_event_type::element_exitted:
        cur_col=this->enabled?col_active:col_disabled;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(element_exitted,this));
        break;

    case gui_event_type::mouse_pressed:
        cur_col=col_clicked;
        break;

    case gui_event_type::mouse_released:
        cur_col=hovered?col_hover:col_active;
        checked=!checked;
        if(this->event_listener)
            this->event_listener->on_event(gui_event(checkbox_state_changed,this));
        break;
    default:
        break;
    }
}
