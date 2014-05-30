#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_static_text.h"

gui_static_text::gui_static_text(gui_environment* env, rect2d<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow):gui_element(env,dimensions)
{
    environment=env;

    m_draw_background=drawbackground;
    draw_shadow = drawshadow;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    this->set_parent(env);
}

gui_static_text::~gui_static_text()
{
}

void gui_static_text::render()
{
    if(this->m_draw_background)
    {
        glBindTexture(GL_TEXTURE_2D,0);
        environment->draw_gui_quad(absolute_rect);
    }
    this->environment->get_font_renderer()->render_string(this->m_text,glm::vec2(this->absolute_rect.x+1,this->absolute_rect.y+2),this->m_text_color,draw_shadow);

    this->render_children();
}

void gui_static_text::set_text(const std::wstring &text)
{
    this->m_text=text;
}
