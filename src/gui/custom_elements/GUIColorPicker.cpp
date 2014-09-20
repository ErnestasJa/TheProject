#include "Precomp.h"
#include "GUIColorPicker.h"
#include "gui/GUI.h"
#include "resources/image.h"
#include "resources/ImageLoader.h"
#include "opengl/texture.h"
#include "Application/AppContext.h"

GUIColorPicker::GUIColorPicker(GUIEnvironment* env, Rect2D<int> dimensions, bool drawbackground):GUIElement(env,dimensions)
{
    this->Type=GUIET_PANE;
    environment=env;

    colRGB=glm::vec4(255,255,255,255); //white
    colHSV=glm::vec3(0,0,1); //white

    cursorPos=glm::vec2(0,0);

    if(drawbackground)
    {
        bg=new gui_pane(env,Rect2D<int>(0,0,dimensions.w+64+16,dimensions.h+32));
        bg->SetParent(this);
        bg->SetListening(false);
    }

    imgBuf=share(new image());
    imgBuf->Init(dimensions.w,dimensions.h,3);
    texBuf=share(new Texture());
    texBuf->Init(imgBuf);

    picker=new gui_image(env,Rect2D<int>(16,16,dimensions.w,dimensions.h),texBuf);
    picker->SetParent(this);
    picker->SetListening(true);

    image_loader* ldr=new image_loader(env->GetContext()->_logger);
    TexturePtr cursorTex=share(new Texture());
    cursorTex->Init(ldr->load("res/cpcurs.png"));
    cursor=new gui_image(env,Rect2D<int>(cursorPos.x,cursorPos.y,8,8),cursorTex);
    cursor->SetParent(picker);
    cursor->SetListening(false);

    sat=new gui_slider(env,Rect2D<int>(16,dimensions.h+16+4,dimensions.w,8),0,1,1,false);
    sat->SetParent(this);

    ebR=new gui_edit_box(env,Rect2D<int>(picker->GetRelativeRect().w+16,picker->GetRelativeRect().y+32,32,16),L"255",glm::vec4(1,0,0,1));
    ebR->SetParent(picker);
    ebG=new gui_edit_box(env,Rect2D<int>(picker->GetRelativeRect().w+16,picker->GetRelativeRect().y+56,32,16),L"255",glm::vec4(0,1,0,1));
    ebG->SetParent(picker);
    ebB=new gui_edit_box(env,Rect2D<int>(picker->GetRelativeRect().w+16,picker->GetRelativeRect().y+80,32,16),L"255",glm::vec4(0,0,1,1));
    ebB->SetParent(picker);

    btnSet=new gui_button(env,Rect2D<int>(picker->GetRelativeRect().w+16,picker->GetRelativeRect().y+104,32,16),L"Set");
    btnSet->SetParent(picker);

    GenerateHSVMap(sat->get_value());
    UpdateValues();
    UpdateAbsolutePos();
    this->SetListening(false);
    this->SetParent(env);
}

GUIColorPicker::~GUIColorPicker()
{
    //dtor
}

void GUIColorPicker::GenerateHSVMap(float s)
{
    loop(x,imgBuf->width)
    {
        loop(y,imgBuf->height)
        {
            glm::vec3 co=helpers::hsv2rgb(360.f/imgBuf->width * x,s,1.f/imgBuf->height*y);
            imgBuf->SetPixel(x,y,255.f*co.x,255.f*co.y,255.f*co.z);
        }
    }
    texBuf->SetSubImage2D(imgBuf->data,0,0,imgBuf->width,imgBuf->height);
}

bool GUIColorPicker::OnEvent(const GUIEvent &e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.GetType())
    {
    case mouse_pressed:
    case mouse_dragged:
        if(e.get_element()==picker)
        {
            float mx=environment->get_mouse_pos().x;
            float my=environment->get_mouse_pos().y;
            if(picker->GetAbsoluteRect().is_point_inside(mx,my))
            {
                float x=mx-absolute_rect.x-picker->GetRelativeRect().x-4;
                float y=my-absolute_rect.y-picker->GetRelativeRect().y-4;
                cursorPos=glm::vec2(x,y);
                cursor->Move(cursorPos);
//                cursor->GetRelativeRect().clip(picker->GetAbsoluteRect());
//                cursor->UpdateAbsolutePos();

                BringToFront(cursor);
                UpdateValues();
            }
        }
        break;
    case mouse_released:
        break;
    case mouse_moved:
        break;
    case button_pressed:
        break;
    case button_released:
        if(e.get_element()==btnSet)
        {
            uint32_t r=_wtoi(ebR->get_text().c_str());
            uint32_t g=_wtoi(ebG->get_text().c_str());
            uint32_t b=_wtoi(ebB->get_text().c_str());
            colRGB=glm::vec4(r,g,b,255);
        }
        break;
    case scrollbar_changed:
        GenerateHSVMap(sat->get_value());
        UpdateValues();
        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void GUIColorPicker::UpdateValues()
{
    colRGB=imgBuf->GetPixel(cursorPos.x+4,imgBuf->height-1-cursorPos.y-4);

    ebR->set_text(helpers::to_wstr(colRGB.x));
    ebG->set_text(helpers::to_wstr(colRGB.y));
    ebB->set_text(helpers::to_wstr(colRGB.z));
}

glm::vec4 GUIColorPicker::GetColorRGB()
{
    return colRGB;
}

void GUIColorPicker::Render()
{
    RenderChildren();
    environment->draw_sliced_gui_quad(Rect2D<int>(absolute_rect.x+absolute_rect.w+32,absolute_rect.y+16,32,32),helpers::color255(colRGB.x,colRGB.y,colRGB.z,colRGB.w));
}
