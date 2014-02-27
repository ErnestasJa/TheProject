#include "precomp.h"

#include "gui_environment.h"
#include "font_renderer.h"

#include "gui_edit_box.h"
#include "font.h"

gui_edit_box::gui_edit_box(gui_environment* env, rect2d<int> dimensions, std::string text, glm::vec4 text_color, bool drawbackground, bool drawshadow):gui_element(env,dimensions)
{
    environment=env;

    blinktimer=curspos=lastkey=reptimer=sx=0;
    font_size=env->get_font_renderer()->get_default_font()->avgheight;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    _mx=absolute_rect.x + 5;
    _mw=absolute_rect.w - font_size - 5;
    _my=absolute_rect.y + (absolute_rect.h-font_size)/2;

    this->set_parent(env);
}

gui_edit_box::~gui_edit_box()
{
}

void gui_edit_box::render()
{
    blinktimer++;

    if(blinktimer >=50 && blinktimer < 100)
        blink=true;
    if(blinktimer>=100)
    {
        blink=false;
        blinktimer=0;
    }

    _mx=absolute_rect.x + 5;
    _mw=absolute_rect.w - font_size - 5;
    _my=absolute_rect.y + (absolute_rect.h-font_size)/2;

    font_renderer* fr=this->environment->get_font_renderer();

    // RECT
    environment->draw_gui_quad(absolute_rect);

    glEnable(GL_SCISSOR_TEST);
    glScissor(absolute_rect.x, environment->get_absolute_rect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

    if (fr->get_text_dimensions(m_text).x > _mw)
    {
        fr->render_string(m_text,glm::vec2(_mx - sx, _my), m_text_color,false);
        if(focused&&blink)
            fr->render_string("l",glm::vec2(_mx-1 - sx + fr->get_text_dimensions(m_text.substr(0,curspos)).x,_my),m_text_color,false);
    }
    else
    {
        fr->render_string(m_text,glm::vec2(_mx, _my), m_text_color,false);
        if(focused&&blink)
            fr->render_string("l",glm::vec2(_mx-1 +fr->get_text_dimensions(m_text.substr(0,curspos)).x,_my),m_text_color,false);
    }

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

    case mouse_pressed:
        //glm::vec2 mpos=environment->get_mouse_pos();

        break;

    case key_typed:
        curspos++;
        lastkey=environment->get_last_char();
        if(m_text.length()>0)
            m_text=m_text.substr(0,curspos-1)+lastkey+m_text.substr(curspos-1,m_text.length());
        else
            m_text+=lastkey;
        if(environment->get_font_renderer()->get_text_dimensions(m_text).x>_mw)
            sx+=environment->get_font_renderer()->get_text_dimensions(m_text.substr(m_text.length()-1)).x;
        break;

    case key_pressed:
        switch(environment->get_last_key())
        {
        case GLFW_KEY_BACKSPACE:
            if(curspos>0)
            {
                m_text=m_text.substr(0,curspos-1)+m_text.substr(curspos,m_text.length());
                curspos--;
            }

            if(environment->get_font_renderer()->get_text_dimensions(m_text).x>_mw)
                sx-=environment->get_font_renderer()->get_text_dimensions(m_text.substr(m_text.length()-1)).x;
            break;
        case GLFW_KEY_LEFT:
            if(curspos>0)
                curspos--;
            break;
        case GLFW_KEY_RIGHT:
            if(curspos<m_text.length())
                curspos++;
            break;
        default:
            break;
        }

    default:
        break;
    }
}
