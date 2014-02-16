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
    this->m_transform=glm::mat4(1.0f);

    rect2d<float> scaled=environment->scale_gui_rect<float>(absolute_rect.as<float>());

    this->m_transform=glm::translate(this->m_transform,glm::vec3(scaled.x,scaled.y,0));
    this->m_transform=glm::scale(this->m_transform,glm::vec3(scaled.w,scaled.h,0));

    if(this->m_draw)
    {
        glBindTexture(GL_TEXTURE_2D,0);
        environment->draw_gui_quad(absolute_rect,glm::vec4(0.6,0.6,0.8,1));
    }

    this->render_children();
}
