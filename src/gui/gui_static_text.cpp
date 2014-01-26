#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_static_text.h"

gui_static_text::gui_static_text(gui_environment* env, rect2d<int> dimensions, std::string text, glm::vec4 text_color, bool drawbackground):gui_element(env,dimensions.x,dimensions.y,dimensions.w,dimensions.h)
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

    this->set_parent(env);
}

gui_static_text::~gui_static_text()
{
    destroy_children();
}

void gui_static_text::render()
{
    if(this->m_draw_background)
        this->m_background->draw();
    this->environment->get_font_renderer()->render_string(this->m_text,glm::vec2(this->absolute_rect.x,this->absolute_rect.y),this->m_text_color);
}

void gui_static_text::set_text(const std::string &text)
{
    this->m_text=text;
}
