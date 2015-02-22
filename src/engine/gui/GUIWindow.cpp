#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUIWindow.h"

#include "GUIPane.h"
#include "GUIButton.h"

GUIWindow::GUIWindow(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring titlebar_text, bool clip, bool showclose, bool modal, bool movable):GUIElement(env,dimensions)
{
    this->Type=GUIET_WINDOW;
    environment=env;

    this->titlebar_text=titlebar_text;

    this->clip=clip;
    this->showclose=showclose;
    this->modal=modal;
    this->movable=movable;

    this->dragging=false;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    tbr=Rect2D<int>(absolute_rect);
    tbr.resize(tbr.w,20);
    tbr.move(0,1);
    //printf("TBR: %s\n",tbr.to_string().c_str());
    bgr=Rect2D<int>(absolute_rect);
    bgr.resize(bgr.w,bgr.h-20);
    bgr.move(0,20);

    if(showclose)
    {
        close_btn=new GUIButton(env,Rect2D<int>(tbr.w-17,4,14,14),L"X");
        close_btn->SetParent(this);
        close_btn->SetEventListener(this);
    }

    this->SetParent(env);
}

GUIWindow::~GUIWindow()
{
}

void GUIWindow::Render()
{
    glBindTexture(GL_TEXTURE_2D,0);

    environment->draw_sliced_gui_quad(tbr,gui_skin_titlebar);
    environment->draw_sliced_gui_quad(bgr,gui_skin_background);
    environment->get_font_renderer()->RenderString(L"['s]"+titlebar_text+L"[s']",glm::ivec2(tbr.x+6,tbr.y+6));

    this->absolute_rect.move(0,20);
    UpdateAbsolutePos();
    this->RenderChildren();
    this->absolute_rect.move(0,-20);
    UpdateAbsolutePos();
}

bool GUIWindow::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.GetType())
    {
    case mouse_pressed:
        ds=environment->get_mouse_pos();
        if(movable&&dragging==false&&tbr.is_point_inside((int)ds.x,(int)ds.y)==true)
            dragging=true;
        break;
    case mouse_released:
        if(dragging) dragging=false;
        break;
    case mouse_dragged:
        if(movable&&dragging)
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
            this->SetVisible(false);
            GUI_FIRE_EVENT(GUIEvent(window_closed,this,this))
        }

        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void GUIWindow::move(glm::vec2 delta)
{
    this->relative_rect.x+=delta.x;
    this->relative_rect.y+=delta.y;
    if(this->clip)
    {
        Rect2D<int> par=parent->GetAbsoluteRect();
        relative_rect.clip(par);
    }
    UpdateAbsolutePos();
    tbr=Rect2D<int>(absolute_rect);
    tbr.resize(tbr.w,20);
    tbr.move(0,1);
    bgr=Rect2D<int>(absolute_rect);
    bgr.resize(bgr.w,bgr.h-20);
    bgr.move(0,20);
    ds=environment->get_mouse_pos();
}
