#pragma once

#include "gui/GUIElement.h"

class Texture;
template <typename T>
class Rect2D;

class gui_image:public GUIElement
{
private:
    std::shared_ptr<Texture> m_tex;
    bool m_multichannel;
public:
    gui_image(GUIEnvironment* env, Rect2D<int> dimensions, std::shared_ptr<Texture> tex,bool multichannel=true);
    virtual ~gui_image();

    void SetImage(std::shared_ptr<Texture> tex);

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
