#pragma once

#include "gui/GUIElement.h"

class quad;
class shader;
template <typename T>
class Rect2D;

class gui_static_text;

class gui_checkbox:public GUIElement
{
private:
    std::string cross;

    bool checked;

    uint32_t cur_style;
public:
    gui_checkbox(GUIEnvironment* env, Rect2D<int> dimensions, bool checked);
    virtual ~gui_checkbox();

    void Render();

    bool OnEvent(const GUIEvent & e);

    bool is_checked()
    {
        return checked;
    }
protected:
};

