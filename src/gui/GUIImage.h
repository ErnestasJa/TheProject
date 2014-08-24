#pragma once

#include "gui/GUIElement.h"

class Texture;
template <typename T>
class Rect2D;

class gui_image:public GUIElement
{
private:
    std::shared_ptr<Texture> m_tex;
public:
    gui_image(GUIEnvironment* env, Rect2D<int> dimensions, std::shared_ptr<Texture> tex);
    virtual ~gui_image();

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
