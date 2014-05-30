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
    std::wstring m_text;
    uint32_t cur_style;
    glm::vec4 col_active,col_hover,col_clicked,col_disabled,col_text,cur_col;
public:
    gui_button(gui_environment* env, rect2d<int> dimensions, std::wstring text=L"text");
    virtual ~gui_button();

    void render();

    void set_text(const std::wstring &text);

    virtual bool on_event(const gui_event & e);
protected:
};

