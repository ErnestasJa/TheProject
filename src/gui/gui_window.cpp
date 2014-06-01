#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_window.h"

#include "gui_pane.h"
#include "gui_button.h"

gui_window::gui_window(gui_environment* env, rect2d<int> dimensions, std::wstring titlebar_text, bool clip, bool showclose, bool modal):gui_element(env,dimensions)
{
    this->type=GUIET_window;
    environment=env;

    this->titlebar_text=titlebar_text;

    this->clip=clip;
    this->showclose=showclose;
    this->modal=modal;

    this->dragging=false;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    tbr=rect2d<int>(absolute_rect);
    tbr.resize(tbr.w,20);
    printf("TBR: %s\n",tbr.to_string().c_str());
    bgr=rect2d<int>(absolute_rect);
    bgr.resize(bgr.w,bgr.h-20);
    bgr.move(0,20);

    close_btn=new gui_button(env,rect2d<int>(tbr.w-18,2,16,16),L"X");
    close_btn->set_parent(this);
    close_btn->set_event_listener(this);
    close_btn->set_visible(showclose);

    this->set_parent(env);
}

gui_window::~gui_window()
{
}

void gui_window::render()
{
    glBindTexture(GL_TEXTURE_2D,0);

    environment->draw_sliced_gui_quad(tbr,gui_skin_titlebar);
    environment->draw_sliced_gui_quad(bgr);
    environment->get_font_renderer()->render_string(titlebar_text,glm::vec2(tbr.x+6,tbr.y+6),glm::vec4(0,0,0,1),false);

    this->absolute_rect.move(0,20);
    update_absolute_pos();
    this->render_children();
    this->absolute_rect.move(0,-20);
    update_absolute_pos();
}

bool gui_window::on_event(const gui_event & e)
{
    GUI_BEGIN_ON_EVENT(e)

        switch(e.get_type())
        {
        case mouse_pressed:
            ds=environment->get_mouse_pos();
            if(dragging==false&&tbr.is_point_inside((int)ds.x,(int)ds.y)==true)
                dragging=true;
            break;
        case mouse_released:
            if(dragging) dragging=false;
            break;
        case mouse_dragged:
            if(dragging)
            {
                mp=environment->get_mouse_pos();
                dif.x=mp.x-ds.x;
                dif.y=mp.y-ds.y;
                this->move(glm::vec2(dif.x,dif.y));
            }
            break;
        case mouse_moved:
            mp=environment->get_mouse_pos();

            break;
        case button_pressed:

            break;
        case button_released:
            if(e.get_caller()==this->close_btn)
            {
                this->set_visible(false);
                GUI_FIRE_EVENT(gui_event(window_closed,this))
            }

            break;
        default:
            break;
        }

    GUI_END_ON_EVENT(e)
}

void gui_window::move(glm::vec2 delta)
{
    this->relative_rect.x+=delta.x;
    this->relative_rect.y+=delta.y;
    if(this->clip)
    {
        rect2d<int> par=parent->get_absolute_rect();
        relative_rect.clip(par);
    }
    update_absolute_pos();
        tbr=rect2d<int>(absolute_rect);
    tbr.resize(tbr.w,20);
    bgr=rect2d<int>(absolute_rect);
    bgr.resize(bgr.w,bgr.h-20);
    bgr.move(0,20);
    ds=environment->get_mouse_pos();
}
