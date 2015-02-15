#pragma once

#include "gui/GUIElement.h"

class Quad;
class Shader;
template <typename T>
class Rect2D;

class GUIStaticText;

class GUICheckbox:public GUIElement
{
private:
    std::string cross;

    bool checked;

    uint32_t cur_style;
public:
    GUICheckbox(GUIEnvironment* env, Rect2D<int> dimensions, bool checked);
    virtual ~GUICheckbox();

    void Render();

    bool OnEvent(const GUIEvent & e);

    bool is_checked()
    {
        return checked;
    }
protected:
};

