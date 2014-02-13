#pragma once

#include "gui/gui_element.h"

class quad;
class shader;
template <typename T>
class rect2d;
class gui_static_text:public gui_element
{
private:
    std::string m_text;
    glm::vec4 m_text_color;
    bool m_draw_background;

    shader* _material;
    quad* m_background;//background
    bool draw_shadow;
    glm::mat4 m_transform;
public:
    gui_static_text(gui_environment* env, rect2d<int> dimensions, std::string text="text", glm::vec4 text_color=glm::vec4(1,1,1,1), bool drawbackground=false, bool drawshadow=false);
    virtual ~gui_static_text();

    void render();

    void set_text(const std::string &text);

    const std::string &get_text()
    {
        return m_text;
    }

    //this does not do any events
    void on_event(gui_event e)
    {
        return;
    }
protected:
};
