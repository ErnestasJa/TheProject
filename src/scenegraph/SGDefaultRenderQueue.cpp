#include "Precomp.h"
#include "SGDefaultRenderQueue.h"
#include "SGObjects.h"
#include "SGScenegraph.h"

namespace sg
{

sg_default_render_queue::sg_default_render_queue(SGScenegraph * sg)
{
    m_scenegraph = sg;
    m_skybox = nullptr;
}

sg_default_render_queue::~sg_default_render_queue()
{

}

void sg_default_render_queue::add_object(isg_object * obj)
{
    if(obj->get_type()==SGO_MESH || obj->get_type() == SGO_QUAD || obj->get_type() == SGO_LINE_OBJECT)
    {
        m_objects.push_back(obj);
    }
    else if(obj->get_type() == SGO_SKYBOX_OBJECT)
    {
        m_skybox = (sg_skybox_object*)obj;
    }
    else if(obj->get_type()==SGO_LIGHT)
    {
        m_lights.push_back(static_cast<sg_light_object*>(obj));
    }
}

void sg_default_render_queue::set_material(isg_object * obj, sg_material_ptr material)
{
    if(this->m_scenegraph->get_override_material())
        material = this->m_scenegraph->get_override_material();


    sg_material * mat = static_cast<sg_material*>(material.get());

    //mat->set_mat4("m",obj->get_absolute_transform());
    if(obj->get_type()!=SGO_SKYBOX_OBJECT)
        mat->set_mat4("mvp",m_scenegraph->get_shared_mat_vars().view_proj.value * obj->get_absolute_transform());
    else
    {
        glm::mat4 MVP = glm::translate(m_scenegraph->get_shared_mat_vars().view_proj.value*obj->get_absolute_transform(),m_scenegraph->get_active_camera()->get_position());
        MVP = glm::scale(MVP,glm::vec3(100.0));
        mat->set_mat4("mvp",MVP);
    }

    _currentMaterial = material;

    if(_currentMaterial)
        _currentMaterial->set();
}

void sg_default_render_queue::pre_render()
{

}

void sg_default_render_queue::render_all()
{
    pre_render();

    if(m_skybox)
    {
        m_skybox->render(m_scenegraph);
    }

    for(isg_object * obj: m_objects)
    {
        obj->render(m_scenegraph);
    }

    post_render();
}

void sg_default_render_queue::post_render()
{
    clear();
}

void sg_default_render_queue::clear()
{
    m_objects.clear();
    m_lights.clear();
}

}
