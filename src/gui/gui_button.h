#pragma once

#include "gui/gui_element.h"

class quad;
class shader;
template <typename T>
class rect2d;

class gui_static_text;
class gui_image;
class gui_button:public gui_element
{
private:
    std::wstring m_text;
    uint32_t cur_style;
    glm::vec4 col_active,col_hover,col_clicked,col_disabled,col_text,cur_col;
    bool m_toggled,m_toggle;
    gui_image* m_overlay_image;
public:
    gui_button(gui_environment* env, rect2d<int> dimensions, std::wstring text=L"text", bool toggle=false, bool toggle_status=false);
    virtual ~gui_button();

    bool is_toggled(){return m_toggled;}
    void set_toggled(bool b){m_toggled=b;}

    void render();

    void set_text(const std::wstring &text);
    void set_image(gui_image *image);
    void remove_image();

    virtual bool on_event(const gui_event & e);
protected:
};

