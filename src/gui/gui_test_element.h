#ifndef GUI_TEST_ELEMENT_H_INCLUDED
#define GUI_TEST_ELEMENT_H_INCLUDED
#include "gui_element.h"
#include "gui_environment.h"
class gui_test_element:public gui_element
{
private:
    quad* _pane;
    shader* _material;
    glm::mat4 _transform;
    glm::vec3 _color;
public:
    gui_test_element(gui_environment *env, int x, int y, int w, int h):gui_element(x,y,w,h)
    {
        printf("CONSTRUCT My bounds are: %i %i %i %i %i %i %i %i\n",absolute_rect.x,absolute_rect.y,absolute_rect.w,absolute_rect.h,relative_rect.x,relative_rect.y,relative_rect.w,relative_rect.h);
        this->set_parent(env);
        this->environment=env;

        this->_pane=new quad();
        _pane->generate();
    }
    ~gui_test_element()
    {
        this->environment->remove_child(this);
    }
    void on_event(gui_event e)
    {
            switch(e.get_type())
            {
            case element_focused:
                this->event_listener->on_event(e);
                break;
            case element_focus_lost:
                this->event_listener->on_event(e);
                break;
            case element_hovered:
                this->event_listener->on_event(e);
                break;
            case element_exitted:
                this->event_listener->on_event(e);
                break;
            case mouse_pressed:
                this->event_listener->on_event(e);
                break;
            case mouse_released:
                this->event_listener->on_event(e);
                break;
            case mouse_dragged:
                this->event_listener->on_event(gui_event(mouse_dragged,this));
                break;
            default:
                break;
            }
    }

    void render()
    {
        this->_transform=glm::mat4(1.0f);

        float gsx=2.0/1024.0;
        float gsy=2.0/768.0;
        float px=-1+absolute_rect.x*gsx+absolute_rect.w/2*gsx;
        float py=1-absolute_rect.y*gsy-absolute_rect.h/2*gsy;
        float sx=absolute_rect.w/2*gsx;
        float sy=absolute_rect.h/2*gsy;

        this->_transform=glm::translate(this->_transform,glm::vec3(px,py,0));
        this->_transform=glm::scale(this->_transform,glm::vec3(sx,sy,0));
        if(_material)
        {
            _material->set();
            glUniformMatrix4fv(_material->getparam("M"),1,GL_FALSE,glm::value_ptr(_transform));
            glUniform3fv(_material->getparam("C"),1,glm::value_ptr(this->_color));
            glBindTexture(GL_TEXTURE_2D,0);
            this->_pane->draw();
        }
    }

    void set_material(shader* material)
    {
        this->_material=material;
    }

    void set_color(glm::vec3 col)
    {
        this->_color=col;
    }
};

#endif // GUI_TEST_ELEMENT_H_INCLUDED
