#include "Precomp.h"

#include "opengl/Quad.h"
#include "opengl/Shader.h"

#include "GUIEnvironment.h"

#include "GUIPane.h"

gui_pane::gui_pane(GUIEnvironment* env, Rect2D<int> dimensions, bool draw):GUIElement(env,dimensions)
{
    this->Type=GUIET_PANE;
    environment=env;

    m_draw=draw;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    this->SetParent(env);
}

gui_pane::~gui_pane()
{
}

void gui_pane::Render()
{
    if(this->m_draw)
    {
        environment->draw_sliced_gui_quad(absolute_rect,gui_skin_background,true);
    }

    this->RenderChildren();
}
