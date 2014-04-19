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
    if(obj->get_type()==SGO_MESH || obj->get_type() == SGO_QUAD)
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
    if(this->m_scenegraph->get_override_material())
        material = this->m_scenegraph->get_override_material();

    if(material)
    {
        switch(material->mat_type)
        {
        case SGM_STATIC_MESH:
        {
            sg_material_static_mesh * mat = static_cast<sg_material_static_mesh *>(material.get());
            mat->camera_pos = m_scenegraph->get_active_camera()->get_position();
            mat->m = obj->get_absolute_transform();
            mat->mvp = m_scenegraph->get_shared_mat_vars().view_proj.value * mat->m.value;
            mat->n = glm::inverseTranspose(glm::mat3(mat->m.value));


            mat->light_pos=m_lights[0]->get_position();


            if(m_current_material && m_current_material->mat_type==SGM_STATIC_MESH)
            {
                sg_material_static_mesh * cmat = static_cast< sg_material_static_mesh* >(m_current_material.get());
                m_current_material = material;
                mat->set(cmat);
                return;
            }

            break;
        }
        case SGM_POINT_SPRITE:
        {
            sg_material_point_sprite * mat = static_cast<sg_material_point_sprite *>(material.get());

            glm::mat4x4 ViewMatrix = m_scenegraph->get_active_camera()->get_absolute_transform();
            const glm::mat4x4 & bpos = obj->get_absolute_transform();

            ///set uniforms
            mat->vp = m_scenegraph->get_shared_mat_vars().view_proj.value;
            mat->cam_right  = glm::vec3(glm::vec4(1,0,0,1)*ViewMatrix);
            mat->cam_up     = glm::vec3(glm::vec4(0,1,0,1)*ViewMatrix);
            mat->pos        = glm::vec3(bpos[3][0], bpos[3][1], bpos[3][2] );

            break;
        }
        case SGM_VSM_FIRST_PASS:
        {
            sg_material_vsm_first_pass * mat = static_cast<sg_material_vsm_first_pass *>(material.get());

            glm::mat4 P_L  = glm::perspective(50.0f,1.0f,1.0f, 100.0f);
            mat->mvp = P_L * m_lights[0]->get_absolute_transform() * obj->get_absolute_transform();

            break;
        }
        case SGM_TEXTURE_FILTER:
        {
            break;
        }
        case SGM_VSM_FINAL_PASS:
        {
            sg_material_vsm_final_pass * mat = static_cast<sg_material_vsm_final_pass *>(material.get());
            mat->m = obj->get_absolute_transform();
            mat->mv = m_scenegraph->get_shared_mat_vars().view.value * mat->m.value;
            mat->mvp = m_scenegraph->get_shared_mat_vars().view_proj.value * mat->m.value;
            //mat->n = glm::inverseTranspose(glm::mat3(m_scenegraph->get_shared_mat_vars().view.value * mat->m.value));
            mat->n = glm::inverseTranspose(glm::mat3(mat->m.value));

            glm::mat4 P_L  = glm::perspective(50.0f,1.0f,1.0f, 100.0f);
            glm::mat4 B    = glm::scale(glm::translate(glm::mat4(1),glm::vec3(0.5,0.5,0.5)), glm::vec3(0.5,0.5,0.5));
            glm::mat4 BP   = B*P_L;

            mat->s = BP * m_lights[0]->get_absolute_transform();

            mat->light_pos=m_lights[0]->get_position();
            mat->camera_pos = m_scenegraph->get_active_camera()->get_position();



            //mat->s = BP * MV_L;


            break;
        }
        default:
            break;

        }
    }

    m_current_material = material;

    if(m_current_material)
        m_current_material->set();
}

void sg_default_render_queue::pre_render()
{
    std::sort(m_objects.begin(),m_objects.end(),[](isg_object *  a, isg_object *  b)
    {
        if(a->get_material_count()==0)
            return false;

        if(b->get_material_count()==0)
            return true;

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
