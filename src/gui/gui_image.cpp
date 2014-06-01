#include "precomp.h"

#include "opengl/texture.h"

#include "gui_environment.h"

#include "gui_image.h"

gui_image::gui_image(gui_environment* env, rect2d<int> dimensions, std::shared_ptr<texture> tex):gui_element(env,dimensions)
{
    this->type=GUIET_element;
    environment=env;

    absolute_rect=dimensions;
    relative_rect=absolute_rect;

    m_tex=tex;

    this->set_parent(env);
}

gui_image::~gui_image()
{
}

void gui_image::render()
{
    environment->draw_gui_quad(absolute_rect,m_tex,true);

    this->render_children();
}
