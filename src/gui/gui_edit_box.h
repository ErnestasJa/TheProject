#pragma once

#include "gui/gui_element.h"

template <typename T>
class rect2d;
class gui_edit_box:public gui_element
{
private:
    uint32_t blinktimer,curspos,reptimer,sx,font_size;
    float _mx,_mw,_my,_mh; //margins for drawing
    char lastkey;
    bool blink;
    std::string m_text,disp_text;
    glm::vec4 m_text_color;
public:
    gui_edit_box(gui_environment* env, rect2d<int> dimensions, std::string text="text", glm::vec4 text_color=glm::vec4(1,1,1,1), bool drawbackground=false, bool drawshadow=false);
    virtual ~gui_edit_box();

    void render();

    void set_text(const std::string &text);

    const std::string &get_text()
    {
        return m_text;
    }

    void on_event(gui_event e);
protected:
};
