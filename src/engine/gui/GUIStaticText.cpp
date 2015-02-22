#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUIStaticText.h"

GUIStaticText::GUIStaticText(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow):GUIElement(env,dimensions)
{
    this->Type=GUIET_STATIC_TEXT;
    environment=env;

    m_draw_background=drawbackground;
    draw_shadow = drawshadow;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_text=text;
    m_text_color=text_color;

    this->SetParent(env);
}

GUIStaticText::~GUIStaticText()
{
}

void GUIStaticText::Render()
{
    if(this->m_draw_background)
    {
        glBindTexture(GL_TEXTURE_2D,0);
        environment->draw_gui_quad(absolute_rect,gui_skin_background);
    }
    this->environment->get_font_renderer()->RenderString(this->m_text,glm::ivec2(this->absolute_rect.x+1,this->absolute_rect.y+2));

    this->RenderChildren();
}

void GUIStaticText::set_text(const std::wstring &text)
{
    this->m_text=text;
}
