#include "precomp.h"
#include "scenegraph.h"
#include "sg_graphics_manager.h"
#include "sg_default_render_queue.h"
#include "opengl/shader.h"
#include "opengl/mesh.h"
#include "utility/logger.h"

namespace sg
{

scenegraph::scenegraph(logger * l, timer_ptr app_timer):isg_object(nullptr), m_render_queue(nullptr)
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
    m_render_queue = share(new sg_default_render_queue(this));
}

scenegraph::~scenegraph()
{
    //dtor
}

sg_light_object_ptr scenegraph::add_light_object()
{
    sg_light_object_ptr obj = share(new sg_light_object(this));
    add_child(obj);
    return obj;
}

void scenegraph::pre_render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if(m_active_camera)
    {
        m_active_camera->update(this);
        sg_shared_mat_vars & v = m_shared_mat_vars;
        v.view = m_active_camera->get_absolute_transform();
        v.proj = m_active_camera->get_projection();
        v.view_proj = v.proj.value * v.view.value;
    }
}

bool scenegraph::register_for_rendering()
{
    sg_render_queue_ptr rq = this->get_render_queue();

    for(sg_object_ptr obj: m_children)
        rq->add_object(obj.get());

    return true;
}


void scenegraph::render_all()
{
    pre_render();

    register_for_rendering();
    m_render_queue->render_all();
    m_render_queue->clear();

    post_render();
}

sg_graphics_manager_ptr scenegraph::get_graphics_manager()
{
    return m_graphics_manager;
}

sg_render_queue_ptr scenegraph::get_render_queue()
{
    return m_render_queue;
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

            if(pos+1<pmesh->sub_meshes[i].material_name.length())
                image_path = pmesh->sub_meshes[i].material_name.substr(pos+1);

            m_logger->log(LOG_LOG, "full mat_name =%s", pmesh->sub_meshes[i].material_name.c_str());
            m_logger->log(LOG_LOG, "mat_name =%s", mat_name.c_str());
            m_logger->log(LOG_LOG, "image_path =%s", image_path.c_str());

            if(load_textures)
            {
                sg::sg_material_ptr mat = ret->get_material(i);
                if(mat->mat_type == SGM_STATIC_MESH)
                {
                    sg_material_static_mesh * sm_mat = static_cast<sg_material_static_mesh*>(mat.get());

                    if(image_path.length()!=0)
                        sm_mat->mat_texture = m_graphics_manager->load_texture(texture_path + image_path);
                }
            }
        }
    }

    return ret;
}

uint32_t scenegraph::get_type()
{
    return SGO_SCENEGRAPH;
}

void scenegraph::render(scenegraph * sg)
{

}

sg_material_ptr scenegraph::get_material(uint32_t index)
{
    return sg_material_ptr();
}

uint32_t scenegraph::get_material_count()
{
    return 0;
}

const sg_shared_mat_vars & scenegraph::get_shared_mat_vars() const
{
    return m_shared_mat_vars;
}

}
