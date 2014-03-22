#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_event.h"

#include "gui_environment.h"

#include "gui_slider.h"

gui_slider::gui_slider(gui_environment* env, rect2d<int> dimensions, int min, int max, int pos):gui_element(env,dimensions)
{
    environment=env;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_min=min;
    m_max=max;
    m_cur_value=m_old_value=pos;
    m_slider_pos=(int)(dimensions.w/((max-min)/pos));


    this->set_parent(env);
}

gui_slider::~gui_slider()
{
}

void gui_slider::render()
{

    environment->draw_gui_quad(rect2d<int>(absolute_rect.x,absolute_rect.y+absolute_rect.h/4,absolute_rect.w,absolute_rect.h/2),gui_skin_background,true);
    environment->draw_gui_quad(rect2d<int>(absolute_rect.x+m_slider_pos-6,absolute_rect.y,12,absolute_rect.h),gui_skin_background,true);
    this->render_children();
}

void gui_slider::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case mouse_pressed:
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.x - absolute_rect.x);

        if (m_slider_pos >= absolute_rect.w)
            m_slider_pos = absolute_rect.w;
        if (m_slider_pos < 0)
            m_slider_pos = 0;

        m_cur_value = glm::round((float)(((m_max - m_min) * m_slider_pos) / absolute_rect.w));
        if(this->event_listener)
            this->event_listener->on_event(gui_event(scrollbar_changed,this));

        printf("Slider value:%d\n",m_cur_value);
        break;
    case mouse_dragged:
        m_mouse_pos=environment->get_mouse_pos();
        m_slider_pos = (int) (m_mouse_pos.x - absolute_rect.x);

        if (m_slider_pos >= absolute_rect.w)
            m_slider_pos = absolute_rect.w;
        if (m_slider_pos < 0)
            m_slider_pos = 0;


        m_cur_value = glm::round((float)(((m_max - m_min) * m_slider_pos) / absolute_rect.w));
        if(this->event_listener)
            if(m_cur_value!=m_old_value)
            {
                this->event_listener->on_event(gui_event(scrollbar_changed,this));
                m_old_value=m_cur_value;
            }
        printf("Slider value:%d\n",m_cur_value);
        break;
    default:
        break;
    }
}
