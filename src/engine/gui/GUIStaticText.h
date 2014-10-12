#pragma once

#include "gui/GUIElement.h"

class Quad;
class Shader;
template <typename T>
class Rect2D;
class gui_static_text:public GUIElement
{
private:
    std::wstring m_text;
    glm::vec4 m_text_color;
    bool m_draw_background;

    bool draw_shadow;
public:
    gui_static_text(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text=L"text", glm::vec4 text_color=glm::vec4(1,1,1,1), bool drawbackground=false, bool drawshadow=false);
    virtual ~gui_static_text();

    void Render();

    void set_text(const std::wstring &text);

    const std::wstring &get_text()
    {
        return m_text;
    }

    //this does not do any events
    virtual bool OnEvent(const GUIEvent & e)
    {
        return false;
    }
protected:
};
