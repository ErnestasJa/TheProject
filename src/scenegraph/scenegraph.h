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
class sg_graphics_manager;
typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;

class isg_render_queue;
typedef std::shared_ptr<isg_render_queue> sg_render_queue_ptr;

class scenegraph
{
public:
    scenegraph(logger * l, timer_ptr app_timer);
    virtual ~scenegraph();

    sg_object_ptr add_object(sg_object_ptr object);
    sg_light_object_ptr add_light_object();
    sg::sg_mesh_object_ptr load_mesh_object(std::string file, bool load_textures);

    sg_graphics_manager_ptr get_graphics_manager();
    timer_ptr get_timer();

    sg_camera_object_ptr get_active_camera();
    void set_active_camera(sg_camera_object_ptr cam);
    sg_render_queue_ptr get_render_queue();
    const sg_shared_mat_vars & get_shared_mat_vars() const;

    void set_override_material(sg_material_ptr material);
    sg_material_ptr get_override_material();

///rendering
    virtual bool register_objects_for_rendering();
    void render_all();
    void update_all(float delta_time);

protected:
    void pre_render();
    void post_render();

    timer_ptr                   m_timer;
    logger*                     m_logger;
    sg_render_queue_ptr         m_render_queue;
    sg_graphics_manager_ptr     m_graphics_manager;
    sg_material_ptr             m_current_material,
                                m_override_material;
    sg_camera_object_ptr        m_active_camera;
    sg_shared_mat_vars          m_shared_mat_vars;

    std::vector<sg_object_ptr> m_objects;
};

}

#endif // SCENEGRAPH_H
