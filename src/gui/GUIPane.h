#pragma once

#include "gui/GUIElement.h"

class quad;
class shader;
template <typename T>
class Rect2D;
class gui_pane:public GUIElement
{
private:
    bool m_draw;
public:
    gui_pane(GUIEnvironment* env, Rect2D<int> dimensions, bool draw=true);
    virtual ~gui_pane();

    void Render();

    //this does not do any events
    void OnEvent(GUIEvent e)
    {
        if(this->event_listener)
        {
            event_listener->OnEvent(e);
            return;

        }
    }
protected:
};
