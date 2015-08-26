#pragma once

#include "gui/GUIElement.h"

class Texture;
template <typename T>
class Rect2D;

class GUIImage:public GUIElement
{
private:
    std::shared_ptr<Texture> m_tex;
    bool m_multichannel;
public:
    GUIImage(GUIEnvironment* env, Rect2D<int> dimensions, std::shared_ptr<Texture> tex,bool multichannel=true);
    virtual ~GUIImage();

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
