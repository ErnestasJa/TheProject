#ifndef GUI_TEST_ELEMENT_H_INCLUDED
#define GUI_TEST_ELEMENT_H_INCLUDED
#include "gui_element.h"
#include "gui_environment.h"
class gui_test_element:public gui_element
{
public:
    gui_test_element(gui_environment *env, int x, int y, int w, int h):gui_element(x,y,w,h)
    {
        printf("CONSTRUCT My bounds are: %i %i %i %i %i %i %i %i\n",absolute_rect.x,absolute_rect.y,absolute_rect.w,absolute_rect.h,relative_rect.x,relative_rect.y,relative_rect.w,relative_rect.h);
        this->set_parent(env);
        this->environment=env;
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
};

#endif // GUI_TEST_ELEMENT_H_INCLUDED
