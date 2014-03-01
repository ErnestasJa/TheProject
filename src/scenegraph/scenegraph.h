#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "sg_objects.h"
#include "sg_material.h"
#include "sg_shared_mat_vars.h"
#include "utility/timer.h"


class image_loader;
class mesh_loader;

class logger;
struct texture;

namespace sg
{
class sg_graphics_manager; typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
class isg_render_queue; typedef std::shared_ptr<isg_render_queue> sg_render_queue_ptr;

class scenegraph: public isg_object
{
public:
    scenegraph(logger * l, timer_ptr app_timer);
    virtual ~scenegraph();

    sg_light_object_ptr add_light_object();

    sg_graphics_manager_ptr get_graphics_manager();
    timer_ptr get_timer();
    sg_camera_object_ptr get_active_camera();
    const sg_shared_mat_vars & get_shared_mat_vars() const;
    sg_render_queue_ptr get_render_queue();

    void set_active_camera(sg_camera_object_ptr cam);

///loading
public:
    sg::sg_mesh_object_ptr load_mesh_object(std::string file, bool load_textures);

///rendering
    virtual bool register_for_rendering();
    void render_all();
    void update_all(float delta_time);

///matrix
    glm::mat4x4 & get_view_projection_matrix();

///isg_object
    virtual uint32_t get_type();
    virtual void render(scenegraph * sg);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

protected:
    void pre_render();
    void post_render();

    timer_ptr m_timer;
    logger * m_logger;

    sg_render_queue_ptr         m_render_queue;

    sg_graphics_manager_ptr     m_graphics_manager;

    sg_material_ptr             m_current_material;
    sg_camera_object_ptr        m_active_camera;

    sg_shared_mat_vars          m_shared_mat_vars;
};

}

#endif // SCENEGRAPH_H
