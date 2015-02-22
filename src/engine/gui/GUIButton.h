#pragma once

#include "gui/GUIElement.h"

class Quad;
class Shader;
template <typename T>
class Rect2D;

class GUIStaticText;
class GUIImage;
class GUIButton:public GUIElement
{
private:
    std::wstring m_text;
    uint32_t cur_style;
    glm::vec4 col_active,col_hover,col_clicked,col_disabled,col_text,cur_col;
    bool m_toggled,m_toggle;
    GUIImage* m_overlay_image;
public:
    GUIButton(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text=L"text", bool toggle=false, bool toggle_status=false);
    virtual ~GUIButton();

    bool is_toggled(){return m_toggled;}
    void set_toggled(bool b){m_toggled=b;}

    void Render();

    void set_text(const std::wstring &text);
    void set_image(GUIImage *image);
    void remove_image();

    virtual bool OnEvent(const GUIEvent & e);
protected:
};

