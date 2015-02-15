#include "Precomp.h"

#include "opengl/Texture.h"

#include "GUIEnvironment.h"

#include "GUIImage.h"

GUIImage::GUIImage(GUIEnvironment* env, Rect2D<int> dimensions, std::shared_ptr<Texture> tex,bool multichannel):GUIElement(env,dimensions)
{
    this->Type=GUIET_ELEMENT;
    environment=env;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_tex=tex;
    m_multichannel=multichannel;

    //it's an image...
    this->SetListening(false);

    this->SetParent(env);
}

GUIImage::~GUIImage()
{
}

void GUIImage::SetImage(std::shared_ptr<Texture> tex)
{
    if(tex!=nullptr)
    m_tex=tex;
    else
        return;
}

void GUIImage::Render()
{
    environment->draw_gui_quad(absolute_rect,m_tex,false,m_multichannel);

    this->RenderChildren();
}
