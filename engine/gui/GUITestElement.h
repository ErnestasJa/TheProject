#ifndef GUI_TEST_ELEMENT_H_INCLUDED
#define GUI_TEST_ELEMENT_H_INCLUDED

#include "GUIElement.h"
#include "GUIEnvironment.h"
class gui_test_element:public GUIElement
{
private:
    Quad* _pane;
    Shader* _material;
    glm::mat4 _transform;
    glm::vec3 _color;
    glm::vec2 mp,ds,dif;
    bool dragging;
public:
    gui_test_element(GUIEnvironment *env, int x, int y, int w, int h):GUIElement(env,x,y,w,h)
    {
        this->SetParent(env);

        this->_pane=new Quad();
        _pane->Init();

        dragging=false;
    }
    ~gui_test_element()
    {
        this->environment->RemoveChild(this);
    }
    void OnEvent(GUIEvent e)
    {
        switch(e.GetType())
        {
        case element_focused:
            this->event_listener->OnEvent(e);
            break;
        case element_focus_lost:
            this->event_listener->OnEvent(e);
            break;
        case element_hovered:
            this->event_listener->OnEvent(e);
            break;
        case element_exitted:
            this->event_listener->OnEvent(e);
            break;
        case mouse_pressed:
            ds=environment->get_mouse_pos();
            if(dragging==false&&absolute_rect.is_point_inside(ds.x,ds.y))
                dragging=true;
            this->event_listener->OnEvent(GUIEvent(mouse_pressed,this));
            break;
        case mouse_released:
            if(dragging) dragging=false;
            this->event_listener->OnEvent(GUIEvent(mouse_released,this));
            break;
        case mouse_dragged:
            mp=environment->get_mouse_pos();
            dif.x=mp.x-ds.x;
            dif.y=mp.y-ds.y;
            this->move(glm::vec2(dif.x,dif.y),true);
            this->event_listener->OnEvent(GUIEvent(mouse_dragged,this));
            break;
        default:
            break;
        }
    }

    void Render()
    {
        this->_transform=glm::mat4(1.0f);

        float gsx=environment->get_gui_scale().x;
        float gsy=environment->get_gui_scale().y;
        float px=-1+absolute_rect.x*gsx+absolute_rect.w/2*gsx;
        float py=1-absolute_rect.y*gsy-absolute_rect.h/2*gsy;
        float sx=absolute_rect.w/2*gsx;
        float sy=absolute_rect.h/2*gsy;

        this->_transform=glm::translate(this->_transform,glm::vec3(px,py,0));
        this->_transform=glm::scale(this->_transform,glm::vec3(sx,sy,0));
        if(_material)
        {
            _material->Set();
            glUniformMatrix4fv(_material->getparam("M"),1,GL_FALSE,glm::value_ptr(_transform));
            glUniform3fv(_material->getparam("C"),1,glm::value_ptr(this->_color));
            glBindTexture(GL_TEXTURE_2D,0);
            this->_pane->Render();
        }
        this->RenderChildren();
    }

    void set_material(Shader* material)
    {
        this->_material=material;
    }

    void set_color(glm::vec3 col)
    {
        this->_color=col;
    }

    void move(glm::vec2 delta,bool clip)
    {
        this->relative_rect.x+=delta.x;
        this->relative_rect.y+=delta.y;
        if(clip)
        {
            Rect2D<int> par=parent->GetAbsoluteRect();
            relative_rect.clip(par);
        }
        UpdateAbsolutePos();
        ds=environment->get_mouse_pos();
    }
};

#endif // GUI_TEST_ELEMENT_H_INCLUDED
