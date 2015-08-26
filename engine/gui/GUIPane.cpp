#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUIPane.h"

GUIPane::GUIPane(GUIEnvironment* env, Rect2D<int> dimensions, bool draw):GUIElement(env,dimensions)
{
    this->Type=GUIET_PANE;
    environment=env;

    m_draw=draw;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    this->SetParent(env);
}

GUIPane::~GUIPane()
{
}

void GUIPane::Render()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(absolute_rect.x, environment->GetAbsoluteRect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

    if(this->m_draw)
    {
        environment->draw_sliced_gui_quad(absolute_rect,gui_skin_background);
    }

    this->RenderChildren();

    glDisable(GL_SCISSOR_TEST);
}
