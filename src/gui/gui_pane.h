#pragma once

#include "gui/gui_element.h"

class quad;
class shader;
template <typename T>
class rect2d;
class gui_pane:public gui_element
{
private:
    bool m_draw;

    bool draw_shadow;
    glm::mat4 m_transform;
public:
    gui_pane(gui_environment* env, rect2d<int> dimensions, bool draw=true);
    virtual ~gui_pane();

    void render();

    //this does not do any events
    void on_event(gui_event e)
    {
        return;
    }
protected:
};
