#pragma once

#include "gui/gui_element.h"

class quad;
class shader;
template <typename T>
class rect2d;

class gui_static_text;

class gui_button:public gui_element
{
private:
    std::string m_text;

    glm::vec4 col_active,col_hover,col_clicked,col_disabled,col_text,cur_col;
public:
    gui_button(gui_environment* env, rect2d<int> dimensions, std::string text="text");
    virtual ~gui_button();

    void render();

    void set_text(const std::string &text);

    void on_event(gui_event e);
protected:
};

