#pragma once

#include "gui/GUIElement.h"

class Quad;
class Shader;
template <typename T>
class Rect2D;
class GUIPane:public GUIElement
{
private:
    bool m_draw;
public:
    GUIPane(GUIEnvironment* env, Rect2D<int> dimensions, bool draw=true);
    virtual ~GUIPane();

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
