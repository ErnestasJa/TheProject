#include "precomp.h"

#include "gui_environment.h"
#include "font_renderer.h"

#include "gui_edit_box.h"
#include "font.h"

gui_edit_box::gui_edit_box(gui_environment* env, rect2d<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow, bool clearonsubmit):gui_element(env,dimensions)
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

    this->clearonsubmit=clearonsubmit;

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
    _mw=absolute_rect.w - 5;
    _my=absolute_rect.y + (absolute_rect.h-font_size)/2;

    sx=_mw-1-environment->get_font_renderer()->get_text_dimensions(m_text.substr(0,curspos)).x;
    if(sx>0)
        sx=0;

    font_renderer* fr=this->environment->get_font_renderer();

    // RECT
    environment->draw_gui_quad(absolute_rect);

    glEnable(GL_SCISSOR_TEST);
    glScissor(absolute_rect.x, environment->get_absolute_rect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

        fr->render_string(m_text,glm::vec2(_mx + sx, _my), m_text_color,false);
        if(focused&&blink)
            fr->render_string(L"l",glm::vec2(_mx-1 + sx + fr->get_text_dimensions(m_text.substr(0,curspos)).x,_my),m_text_color,false);

    glDisable(GL_SCISSOR_TEST);

    this->render_children();
}

void gui_edit_box::set_text(const std::wstring &text)
{
    this->m_text=text;
}

bool gui_edit_box::on_event(const gui_event & e)
{
    GUI_BEGIN_ON_EVENT(e)

        std::wstring temp;
        switch(e.get_type())
        {
        case element_focused:
            break;

        case mouse_pressed:
            //glm::vec2 mpos=environment->get_mouse_pos();

            break;

        case key_typed:
            lastkey=environment->get_last_char();
            temp=L"";
            temp+=lastkey;
            add_text(curspos,temp);
            break;

        case key_pressed:
            switch(environment->get_last_key())
            {
            case GLFW_KEY_ENTER:
                if(clearonsubmit)
                {
                    this->m_text.clear();
                }
                this->set_focused(false);
                if(this->event_listener)
                {
                    GUI_FIRE_EVENT(gui_event(textbox_submit,this))
                    GUI_FIRE_EVENT(gui_event(element_focus_lost,this))
                }
                break;
            case GLFW_KEY_BACKSPACE:
                if(m_text.length()>0)
                    remove_text(curspos,1);
                break;
            case GLFW_KEY_DELETE:
                if(curspos<m_text.length())
                    remove_text(curspos+1,1);
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
            break;

        case text_paste:
            add_text(curspos,environment->get_clipboard());
            break;

        default:
            break;
        }

    GUI_END_ON_EVENT(e)
}

void gui_edit_box::add_text(int32_t index,std::wstring text)
{
    curspos=index;

    if(m_text.length()>0)
        m_text=m_text.substr(0,curspos)+text+m_text.substr(curspos,m_text.length());
    else
        m_text+=text;

    curspos+=text.length();
}

void gui_edit_box::remove_text(int32_t index, int32_t length)
{
    curspos=index;

    if(curspos-length>0)
    {
        m_text=m_text.substr(0,curspos-length)+m_text.substr(curspos,m_text.length());
        curspos-=length;
    }
    else if(curspos>0)
    {
        m_text=m_text.substr(0,curspos-1)+m_text.substr(curspos,m_text.length());
        curspos--;
    }
}
