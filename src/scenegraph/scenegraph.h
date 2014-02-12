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

public:
    sg_graphics_manager_ptr get_graphics_manager();

public:
    virtual void add_object(sg_object_ptr obj);
    virtual void render_all();

    sg_camera_object_ptr get_active_camera();
    void set_active_camera(sg_camera_object_ptr cam);

    timer_ptr get_timer();

    virtual void on_set_material(sg_material_ptr mat);
///loading
public:
    sg::sg_mesh_object_ptr load_mesh_object(std::string file, bool load_textures);

///matrix
public:
    glm::mat4x4 & get_view_projection_matrix();

protected:
    virtual void pre_render();

    virtual void post_render();

protected:
    timer_ptr m_timer;

    logger * m_logger;
    sg_graphics_manager_ptr     m_graphics_manager;

    sg_camera_object_ptr        m_active_camera;
    std::vector<sg_object_ptr>  m_objects;
    sg_material_ptr             m_current_material;

    glm::mat4 M, V, P, VP, MVP;
};

}

#endif // SCENEGRAPH_H
