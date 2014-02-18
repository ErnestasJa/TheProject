#include "precomp.h"

#include "opengl/quad.h"
#include "opengl/shader.h"

#include "gui_environment.h"

#include "gui_pane.h"

gui_pane::gui_pane(gui_environment* env, rect2d<int> dimensions, bool draw):gui_element(env,dimensions)
{
    environment=env;

    m_draw=draw;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    this->set_parent(env);
}

gui_pane::~gui_pane()
{
}

void gui_pane::render()
{
    if(this->m_draw)
    {
        environment->draw_gui_quad(absolute_rect,gui_skin_background,true);
    }

    this->render_children();
}
