#pragma once

#include "gui/gui_element.h"

class quad;
class shader;
template <typename T>
class rect2d;

class gui_static_text;

class gui_checkbox:public gui_element
{
private:
    std::string cross;

    bool checked;

    uint32_t cur_style;
public:
    gui_checkbox(gui_environment* env, rect2d<int> dimensions, bool checked);
    virtual ~gui_checkbox();

    void render();

    void on_event(gui_event e);

    bool is_checked()
    {
        return checked;
    }
protected:
};

