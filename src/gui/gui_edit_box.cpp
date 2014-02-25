#include "precomp.h"

#include "gui_environment.h"
#include "font_renderer.h"

#include "gui_edit_box.h"

gui_edit_box::gui_edit_box(gui_environment* env, rect2d<int> dimensions, std::string text, glm::vec4 text_color, bool drawbackground, bool drawshadow):gui_element(env,dimensions)
{
    environment=env;

    blinktimer=curspos=lastkey=reptimer=sx=0;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    this->set_parent(env);
}

gui_edit_box::~gui_edit_box()
{
}

void gui_edit_box::render()
{

    font_renderer* fr=this->environment->get_font_renderer();

    // RECT
    environment->draw_gui_quad(absolute_rect);

    glEnable(GL_SCISSOR_TEST);
    glScissor(absolute_rect.x, environment->get_absolute_rect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w+1, absolute_rect.h);

    if (fr->get_text_dimensions(m_text).x > absolute_rect.w)
    {
        fr->render_string(m_text,glm::vec2(absolute_rect.x - sx, absolute_rect.y + (absolute_rect.h - 12) / 2), m_text_color,false);
        fr->render_string("l",glm::vec2(absolute_rect.x - sx + fr->get_text_dimensions(m_text).x+1,absolute_rect.y + (absolute_rect.h - 12) / 2),m_text_color,false);
    }
    else
    {
        fr->render_string(m_text,glm::vec2(absolute_rect.x, absolute_rect.y + (absolute_rect.h - 12) / 2), m_text_color,false);
        fr->render_string("l",glm::vec2(absolute_rect.x+fr->get_text_dimensions(m_text).x+1,absolute_rect.y + (absolute_rect.h - 12) / 2),m_text_color,false);
    }

    //if (blink && focused)
    //	fnt.drawString(
    //			m_text,glm::vec2(absoluteRect._x + fnt.getWidth(text.substring(0, curspos))
    //					- sx,
    //			absoluteRect._y + (absoluteRect._h - fnt.getHeight("A"))
    //					/ 2 - 1), "|", this.enabled ? textColor
    //					: Color.lightGray);
    glDisable(GL_SCISSOR_TEST);

    this->render_children();
}

void gui_edit_box::set_text(const std::string &text)
{
    this->m_text=text;
}

void gui_edit_box::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case element_focused:
        break;

    case key_typed:
        m_text+=environment->get_last_char();
        if(environment->get_font_renderer()->get_text_dimensions(m_text).x>absolute_rect.w)
            sx+=environment->get_font_renderer()->get_text_dimensions(m_text.substr(m_text.length()-1)).x;
        break;

    case key_pressed:
        switch(environment->get_last_key())
        {
        case GLFW_KEY_BACKSPACE:
            if(environment->get_font_renderer()->get_text_dimensions(m_text).x>absolute_rect.w)
                sx-=environment->get_font_renderer()->get_text_dimensions(m_text.substr(m_text.length()-1)).x;
            m_text=m_text.substr(0,m_text.length()-1);
            break;
        default:
            break;
        }

    default:
        break;
    }
}
