#ifndef GUICOLORPICKER_H
#define GUICOLORPICKER_H

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;

class GUIStaticText;
class GUIImage;
class GUIEditBox;
class GUIButton;
class GUIPane;
class GUISlider;
class image;
class Texture;

class GUIColorPicker:public GUIElement
{
private:
    glm::vec4 colRGB;
    glm::vec3 colHSV;
    glm::vec2 cursorPos;
    GUIImage *cursor,*picker;
    GUIPane *bg;
    GUIButton *btnSet;
    GUIEditBox *ebR,*ebG,*ebB;
    GUISlider *sat,*val;
    std::shared_ptr<image> imgBuf;
    std::shared_ptr<Texture> texBuf;

    void UpdateValues();
public:
    GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions,bool drawbackground=true);
    virtual ~GUIColorPicker();

    void Render();

    glm::vec4 GetColorRGB();
    void SetColorRGB(uint8_t r, uint8_t g, uint8_t b);

//    glm::vec3 GetColorHSV();
//    void SetColorHSV(uint32_t H, float S, float V);

    void GenerateHSVMap(float s);

    virtual bool OnEvent(const GUIEvent & e);
protected:
};

#endif // GUICOLORPICKER_H
