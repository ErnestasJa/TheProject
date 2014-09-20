#ifndef GUICOLORPICKER_H
#define GUICOLORPICKER_H

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;

class gui_static_text;
class gui_image;
class gui_edit_box;
class gui_button;
class gui_pane;
class gui_slider;
class image;
class Texture;

class GUIColorPicker:public GUIElement
{
private:
    glm::vec4 colRGB;
    glm::vec3 colHSV;
    glm::vec2 cursorPos;
    gui_image *cursor,*picker;
    gui_pane *bg;
    gui_button *btnSet;
    gui_edit_box *ebR,*ebG,*ebB;
    gui_slider *sat,*val;
    std::shared_ptr<image> imgBuf;
    std::shared_ptr<Texture> texBuf;

    void UpdateValues();
public:
    GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions);
    virtual ~GUIColorPicker();

    void Render();

    glm::vec4 GetColorRGB();
//    void SetColorRGB(uint8_t r, uint8_t g, uint8_t b);
//
//    glm::vec3 GetColorHSV();
//    void SetColorHSV(uint32_t H, float S, float V);

    void GenerateHSVMap(float s);

    virtual bool OnEvent(const GUIEvent & e);
protected:
};

#endif // GUICOLORPICKER_H
