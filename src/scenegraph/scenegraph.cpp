#include "precomp.h"
#include "scenegraph.h"
#include "opengl/shader.h"

namespace sg
{

scenegraph::scenegraph()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);
}

scenegraph::~scenegraph()
{
    //dtor
}

void scenegraph::add_object(sg_object_ptr obj)
{
    m_objects.push_back(obj);
}

void scenegraph::pre_render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(m_active_camera)
    {
        VP = m_active_camera->get_projection() * m_active_camera->get_transform();
        //m_active_camera->
    }
}

void scenegraph::render_all()
{
    pre_render();

    for(uint32_t i = 0; i < m_objects.size(); i++)
    {
        MVP = VP * m_objects[i]->get_transform();
        m_objects[i]->render(this);
    }

    post_render();
}

void scenegraph::on_set_material(const sg_material & mat)
{
    if(m_current_material != mat)
    {
        m_current_material = mat;
        m_current_material.set();

            //m_current_material->m_shader
    }

    if(m_current_material.mat_shader)
        glUniformMatrix4fv(m_current_material.mat_shader->getparam("MVP"),1,GL_FALSE,glm::value_ptr(MVP));
}

sg_camera_object_ptr scenegraph::get_active_camera()
{
    return m_active_camera;
}

void scenegraph::set_active_camera(sg_camera_object_ptr cam)
{
    m_active_camera = cam;
}

void scenegraph::post_render()
{

}

}
