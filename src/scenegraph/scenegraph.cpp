#include "precomp.h"
#include "scenegraph.h"
#include "sg_graphics_manager.h"
#include "opengl/shader.h"
#include "opengl/mesh.h"
#include "utility/logger.h"

namespace sg
{

scenegraph::scenegraph(logger * l, timer_ptr app_timer)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);

    m_logger = l;
    m_graphics_manager = share(new sg_graphics_manager(l));
    m_timer = app_timer;
}

scenegraph::~scenegraph()
{
    //dtor
}

void scenegraph::add_object(sg_object_ptr obj)
{
    m_objects.push_back(obj);
}

sg_light_object_ptr scenegraph::add_light_object()
{
    sg_light_object_ptr obj = share(new sg_light_object(this));
    m_lights.push_back(obj);
    return obj;
}

void scenegraph::pre_render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(m_active_camera)
    {
        m_active_camera->update(this);
        V = m_active_camera->get_absolute_transform();
        P = m_active_camera->get_projection();
        VP = m_active_camera->get_projection() * m_active_camera->get_absolute_transform();
        //m_active_camera->
    }
}

void scenegraph::render_all()
{
    pre_render();

    for(uint32_t i = 0; i < m_objects.size(); i++)
    {
        M = m_objects[i]->get_absolute_transform();
        MVP = P*(V*M);
        m_objects[i]->render(this);
    }

    post_render();
}

void scenegraph::on_set_material(sg_material_ptr mat)
{
    m_current_material = nullptr;
    m_current_material = mat;
    m_current_material->set(this);
}

glm::mat4x4 & scenegraph::get_view_projection_matrix()
{
    return VP;
}

sg_graphics_manager_ptr scenegraph::get_graphics_manager()
{
    return m_graphics_manager;
}

std::vector<sg_light_object_ptr> & scenegraph::get_lights()
{
    return m_lights;
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


/// LOADING
#include "opengl/mesh.h"
sg::sg_mesh_object_ptr scenegraph::load_mesh_object(std::string file, bool load_textures)
{
    sg::sg_mesh_object_ptr ret;
    mesh_ptr pmesh;

    pmesh = m_graphics_manager->get_mesh_loader()->load(file);

    if(pmesh)
    {
        ret = share(new sg::sg_mesh_object(this,pmesh));

        std::string texture_path = file.substr(0,file.rfind("/")+1);
        m_logger->log(LOG_LOG, "Texture path =%s", texture_path.c_str());

        for(uint32_t i=0; i<ret->get_material_count(); i++)
        {
            std::string mat_name, image_path;
            uint32_t pos = pmesh->sub_meshes[i].material_name.find("|");
            mat_name = pmesh->sub_meshes[i].material_name.substr(0,pos-1);
            image_path = pmesh->sub_meshes[i].material_name.substr(pos+1);

            m_logger->log(LOG_LOG, "pos =%u", pos);
            m_logger->log(LOG_LOG, "mat_name =%s", mat_name.c_str());
            m_logger->log(LOG_LOG, "image_path =%s", image_path.c_str());

            if(load_textures)
            {
                sg::sg_material_ptr mat = ret->get_material(i);
                if(mat->mat_type == SGMT_STATIC_MESH)
                {
                    sg_material_static_mesh * sm_mat = static_cast<sg_material_static_mesh*>(mat.get());
                    sm_mat->mat_texture = m_graphics_manager->load_texture(texture_path + image_path);

                }
            }
        }
    }

    return ret;
}

}
