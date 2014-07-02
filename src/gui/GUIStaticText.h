#pragma once

#include "gui/GUIElement.h"

class quad;
class shader;
template <typename T>
class rect2d;
class gui_static_text:public gui_element
{
private:
    std::wstring m_text;
    glm::vec4 m_text_color;
    bool m_draw_background;

    bool draw_shadow;
public:
    gui_static_text(gui_environment* env, rect2d<int> dimensions, std::wstring text=L"text", glm::vec4 text_color=glm::vec4(1,1,1,1), bool drawbackground=false, bool drawshadow=false);
    virtual ~gui_static_text();

    void render();

    void set_text(const std::wstring &text);

    const std::wstring &get_text()
    {
        return m_text;
    }

    //this does not do any events
    virtual bool on_event(const gui_event & e)
    {
        return false;
    }
protected:
};
