#pragma once

#include "gui/GUIElement.h"

class quad;
class shader;
template <typename T>
class rect2d;
class gui_pane:public gui_element
{
private:
    bool m_draw;
public:
    gui_pane(gui_environment* env, rect2d<int> dimensions, bool draw=true);
    virtual ~gui_pane();

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
