#include "precomp.h"
#include "sg_default_render_queue.h"
#include "sg_objects.h"
#include "scenegraph.h"

namespace sg
{

sg_default_render_queue::sg_default_render_queue(scenegraph * sg)
{
    m_scenegraph = sg;
}

sg_default_render_queue::~sg_default_render_queue()
{

}

void sg_default_render_queue::add_object(isg_object * obj)
{
    if(obj->get_type()==SGO_MESH)
    {
        m_objects.push_back(obj);
    }
    else if(obj->get_type()==SGO_LIGHT)
    {
        m_lights.push_back(static_cast<sg_light_object*>(obj));
    }
}

void sg_default_render_queue::set_material(isg_object * obj, sg_material_ptr material)
{
    if(m_current_material)
    {
        // TODO (Ernestas#1#): do some optimizations in future (set only changed uniforms)
        m_current_material = material;
    }
    else
    {
       m_current_material = material;
    }

    if(m_current_material)
    {
        switch(m_current_material->mat_type)
        {
        case SGM_STATIC_MESH:
        {
            sg_material_static_mesh * mat = static_cast<sg_material_static_mesh *>(m_current_material.get());
            mat->camera_pos = m_scenegraph->get_active_camera()->get_position();
            mat->m = obj->get_absolute_transform();
            mat->mvp = m_scenegraph->get_shared_mat_vars().view_proj.value * mat->m.value;
            mat->n = glm::inverseTranspose(glm::mat3(mat->m.value));

            if(m_lights.size()>0)
                mat->light_pos=m_lights[0]->get_position();
            else
                mat->light_pos=glm::vec3();

            break;
        }
        case SGM_POINT_SPRITE:
        {
            sg_material_point_sprite * mat = static_cast<sg_material_point_sprite *>(m_current_material.get());

            glm::mat4x4 ViewMatrix = m_scenegraph->get_active_camera()->get_absolute_transform();
            const glm::mat4x4 & bpos = obj->get_absolute_transform();

            ///set uniforms
            mat->vp = m_scenegraph->get_shared_mat_vars().view_proj.value;
            mat->cam_right  = glm::vec3(glm::vec4(1,0,0,1)*ViewMatrix);
            mat->cam_up     = glm::vec3(glm::vec4(0,1,0,1)*ViewMatrix);
            mat->pos        = glm::vec3(bpos[3][0], bpos[3][1], bpos[3][2] );

            break;
        }
        default:
            break;

        }
    }

    if(m_current_material)
        m_current_material->set();
}

void sg_default_render_queue::pre_render()
{
    std::sort(m_objects.begin(),m_objects.end(),[](isg_object *  a, isg_object *  b)
    {
        if(a->get_material_count()==0)
                return false;

        return a->get_material(0)->mat_type<b->get_material(0)->mat_type;
    });


}

void sg_default_render_queue::render_all()
{
    pre_render();

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
