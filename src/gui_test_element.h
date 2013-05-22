#ifndef GUI_TEST_ELEMENT_H_INCLUDED
#define GUI_TEST_ELEMENT_H_INCLUDED
#include "gui_element.h"
#include "gui_environment.h"
class gui_test_element:public gui_element
{
public:
    gui_test_element(gui_environment *env, int x, int y, int w, int h):gui_element(x,y,w,h)
    {
    env->add_child(this);
    this->set_parent(env);
    }
    ~gui_test_element()
    {

    }
    void on_event(gui_event e)
    {
        if(e.get_caller()==this)
        {
            switch(e.get_type())
            {
            case element_hovered:
                this->hovered=true;
                this->event_listener->on_event(e);
                break;
            case element_exitted:
                this->hovered=false;
                this->event_listener->on_event(e);
                break;
            default:
                break;
            }
        }
    }
};

#endif // GUI_TEST_ELEMENT_H_INCLUDED
