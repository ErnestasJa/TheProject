#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "sg_objects.h"
#include "sg_material.h"
#include "utility/timer.h"


class image_loader;
class mesh_loader;

class logger;
struct texture;

namespace sg
{
class sg_graphics_manager; typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;

class scenegraph
{
public:
    scenegraph(logger * l, timer_ptr app_timer);
    virtual ~scenegraph();

    void add_object(sg_object_ptr obj);
    sg_light_object_ptr add_light_object();

    sg_graphics_manager_ptr get_graphics_manager();
    timer_ptr get_timer();
    std::vector<sg_light_object_ptr> & get_lights();
    sg_camera_object_ptr get_active_camera();

    void set_active_camera(sg_camera_object_ptr cam);

///loading
public:
    sg::sg_mesh_object_ptr load_mesh_object(std::string file, bool load_textures);

///rendering
    void render_all();
    void on_set_material(sg_material_ptr mat);

///matrix
    glm::mat4x4 & get_view_projection_matrix();

protected:
    void pre_render();
    void post_render();

    timer_ptr m_timer;

    logger * m_logger;
    sg_graphics_manager_ptr     m_graphics_manager;

    std::vector<sg_object_ptr>  m_objects;
    std::vector<sg_light_object_ptr>  m_lights;
    sg_material_ptr             m_current_material;
    sg_camera_object_ptr        m_active_camera;

    glm::mat4 M, V, P, VP, MVP;
};

}

#endif // SCENEGRAPH_H
