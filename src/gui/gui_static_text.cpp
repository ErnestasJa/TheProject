#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_static_text.h"

gui_static_text::gui_static_text(gui_environment* env, rect2d<int> dimensions, std::string text, glm::vec4 text_color, bool drawbackground):gui_element(env,dimensions)
{
    environment=env;

    m_draw_background=drawbackground;

    if(drawbackground)
    {
        m_background=new quad();
        m_background->generate();
    }
    else
        m_background=nullptr;


    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    _material=env->get_quad_shader();

    this->set_parent(env);
}

gui_static_text::~gui_static_text()
{
    destroy_children();
}

void gui_static_text::render()
{
    this->m_transform=glm::mat4(1.0f);

    rect2d<float> scaled=environment->scale_gui_rect<float>(absolute_rect.as<float>());

    this->m_transform=glm::translate(this->m_transform,glm::vec3(scaled.x,scaled.y,0));
    this->m_transform=glm::scale(this->m_transform,glm::vec3(scaled.w,scaled.h,0));

    if(this->m_draw_background)
    {
        _material->set();
        glUniformMatrix4fv(_material->getparam("M"),1,GL_FALSE,glm::value_ptr(m_transform));
        glUniform3fv(_material->getparam("C"),1,glm::value_ptr(glm::vec4(0.5f)));
        glBindTexture(GL_TEXTURE_2D,0);
        this->m_background->draw();
    }
    this->environment->get_font_renderer()->render_string(this->m_text,glm::vec2(this->absolute_rect.x+1,this->absolute_rect.y+2),this->m_text_color,false);

    this->render_children();
}

void gui_static_text::set_text(const std::string &text)
{
    this->m_text=text;
}
