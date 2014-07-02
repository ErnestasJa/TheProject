#pragma once

#include "gui/GUIElement.h"

class texture;
template <typename T>
class rect2d;

class gui_image:public gui_element
{
private:
    std::shared_ptr<texture> m_tex;
public:
    gui_image(gui_environment* env, rect2d<int> dimensions, std::shared_ptr<texture> tex);
    virtual ~gui_image();

    void render();

    //this does not do any events
    void on_event(gui_event e)
    {
        if(this->event_listener)
        {
            event_listener->on_event(e);
            return;
        }
    }
protected:
};
