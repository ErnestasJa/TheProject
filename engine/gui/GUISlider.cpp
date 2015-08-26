#include "Precomp.h"

#include "utility/Util.h"
#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEvent.h"

#include "GUIEnvironment.h"

#include "GUISlider.h"

GUISlider::GUISlider(GUIEnvironment* env, Rect2D<int> dimensions, float min, float max, float pos, bool vertical):GUIElement(env,dimensions)
{
    this->Type=GUIET_SLIDER;
    environment=env;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_min=min;
    m_max=max;
    m_cur_value=m_old_value=pos;
    if(vertical)
    {
        m_slider_pos=absolute_rect.y-((pos-absolute_rect.h)/(max-min));
    }
    else
    {
        float f=(absolute_rect.w-12)/range();
        m_slider_pos=(m_cur_value-m_min)*f+absolute_rect.h/2;
    }
    m_vertical=vertical;

    this->SetParent(env);
}

GUISlider::~GUISlider()
{
}

void GUISlider::Render()
{
    if(m_vertical)
    {
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x+absolute_rect.w/4,absolute_rect.y,absolute_rect.w/2,absolute_rect.h),gui_skin_scroll_h_bg);
        environment->draw_gui_quad(Rect2D<int>(absolute_rect.x,absolute_rect.y+m_slider_pos-6,absolute_rect.w,12),gui_skin_scroll_h_bar);
    }
    else
    {
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x,absolute_rect.y+absolute_rect.h/4,absolute_rect.w,absolute_rect.h/2),gui_skin_scroll_h_bg);
        environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x+m_slider_pos-6,absolute_rect.y,12,absolute_rect.h),gui_skin_scroll_h_bar);
    }

    this->RenderChildren();
}

bool GUISlider::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.GetType())
    {
    case mouse_pressed:
        handle_mouse();

        if(m_cur_value!=m_old_value)
        {
            GUI_FIRE_EVENT(GUIEvent(scrollbar_changed,this,this))
            m_old_value=m_cur_value;
        }

        break;
    case mouse_dragged:
        handle_mouse();

        if(m_cur_value!=m_old_value)
        {
            GUI_FIRE_EVENT(GUIEvent(scrollbar_changed,this,this))
            m_old_value=m_cur_value;
        }
        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void GUISlider::handle_mouse()
{
    if(m_vertical)
    {
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.y - absolute_rect.y);

        if (m_slider_pos >= absolute_rect.h)
            m_slider_pos = absolute_rect.h;
        if (m_slider_pos < 0)
            m_slider_pos = 0;


        m_cur_value = ((m_max - m_min) * (absolute_rect.y-m_slider_pos)) / absolute_rect.h;
        m_cur_value = glm::clamp(m_cur_value,m_min,m_max);
    }
    else
    {
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.x - absolute_rect.x);

        if (m_slider_pos >= absolute_rect.w)
            m_slider_pos = absolute_rect.w;
        if (m_slider_pos < 0)
            m_slider_pos = 0;

        //m_cur_value = ((m_max - m_min) * m_slider_pos) / absolute_rect.w;
        m_cur_value =  m_slider_pos/absolute_rect.w * range() + m_min;
        m_cur_value = glm::clamp(m_cur_value,m_min,m_max);
    }
    //printf("value: %f\n",m_cur_value);
}

float GUISlider::get_value() const
{
    return m_cur_value;
}

void GUISlider::set_value(float value)
{
    m_cur_value = clamp(value,m_min,m_max);
}
