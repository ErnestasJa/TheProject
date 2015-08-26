#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;

class GUIButton;
class GUIWindow:public GUIElement
{
private:
    bool clip,showclose,modal,dragging,movable;
    std::wstring titlebar_text;

    glm::mat4 m_transform;
    GUIButton* close_btn;


    glm::vec2 mp,dif,ds;
    void move(glm::vec2 delta);

    Rect2D<int> tbr,bgr;
public:
    GUIWindow(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring titlebar_text=L"Window", bool clip=true, bool showclose=true, bool modal=false, bool movable=true);
    virtual ~GUIWindow();

    void Render();

    virtual bool OnEvent(const GUIEvent & e);
protected:
};
