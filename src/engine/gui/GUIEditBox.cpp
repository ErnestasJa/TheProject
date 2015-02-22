#include "Precomp.h"

#include "GUIEnvironment.h"
#include "font_rendering/FontRenderer.h"

#include "GUIEditBox.h"
#include "font_rendering/Font.h"

GUIEditBox::GUIEditBox(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow, bool clearonsubmit):GUIElement(env,dimensions)
{
    this->Type=GUIET_EDITBOX;
    environment=env;

    blinktimer=curspos=lastkey=reptimer=sx=maxlength=0;
    font_size=env->get_font_renderer()->GetCurrentFont()->avgheight;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    curspos=text.length();

    _mx=absolute_rect.x + 5;
    _mw=absolute_rect.w - font_size - 5;
    _my=absolute_rect.y + (absolute_rect.h-font_size)/2;

    this->clearonsubmit=clearonsubmit;

    this->SetParent(env);
}

GUIEditBox::~GUIEditBox()
{
}

void GUIEditBox::Render()
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

    sx=_mw-8-environment->get_font_renderer()->GetTextDimensions(m_text.substr(0,curspos)).x;
    if(sx>0)
        sx=0;

    FontRenderer* fr=this->environment->get_font_renderer();

    // RECT
    if(this->IsFocused())
    {
        environment->draw_sliced_gui_quad(absolute_rect,gui_skin_input_hover);
    }
    else if(this->IsEnabled()==false)
    {
        environment->draw_sliced_gui_quad(absolute_rect,gui_skin_input_disabled);
    }
    else
    {
        environment->draw_sliced_gui_quad(absolute_rect,gui_skin_input_active);
    }


    glEnable(GL_SCISSOR_TEST);
    glScissor(absolute_rect.x, environment->GetAbsoluteRect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

        fr->RenderString(L"['s]"+m_text+L"[s']",glm::ivec2(_mx + sx, _my));
        if(focused&&blink)
            fr->RenderString(L"['s]|[s']",glm::ivec2(_mx-1 + sx + fr->GetTextDimensions(m_text.substr(0,curspos)).x,_my-2));

    glDisable(GL_SCISSOR_TEST);

    this->RenderChildren();
}

void GUIEditBox::set_text(const std::wstring &text)
{
    this->m_text=text;
    curspos=text.length();
}

void GUIEditBox::SetMaxLength(uint32_t length)
{
    maxlength=length;
}

bool GUIEditBox::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

        std::wstring temp;
        switch(e.GetType())
        {
        case element_focused:
            break;

        case mouse_pressed:
            //glm::vec2 mpos=environment->get_mouse_pos();

            break;

        case key_typed:
            if(maxlength==0||m_text.length()<maxlength)
            {
            lastkey=environment->get_last_char();
            //printf("lastchar:%lc %lc\n",lastkey,'è');
            temp=L"";
            temp+=lastkey;
            add_text(curspos,temp);
            }
            break;

        case key_pressed:
            switch(environment->get_last_key())
            {
            case GLFW_KEY_ENTER:
                if(clearonsubmit)
                {
                    this->m_text.clear();
                }
                this->SetFocused(false);
                if(this->event_listener)
                {
                    GUI_FIRE_EVENT(GUIEvent(textbox_submit,this,this))
                    GUI_FIRE_EVENT(GUIEvent(element_focus_lost,this,this))
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

void GUIEditBox::add_text(int32_t index,std::wstring text)
{
    curspos=index;

    if(m_text.length()>0)
        m_text=m_text.substr(0,curspos)+text+m_text.substr(curspos,m_text.length());
    else
        m_text+=text;

    curspos+=text.length();
}

void GUIEditBox::remove_text(int32_t index, int32_t length)
{
    if(index>0&&index-length>=0)
    {
        m_text=m_text.substr(0,index-length)+m_text.substr(index);
        curspos=index-length;
    }
}
