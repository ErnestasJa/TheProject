#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "SGObjects.h"
#include "SGMaterial.h"
#include "SGSharedMatVars.h"
#include "utility/Timer.h"
#include "SGEmptyObject.h"


class image_loader;
class mesh_loader;

class Logger;
struct texture;

struct trigger
{
    std::string name;
    sg::sg_empty_object_ptr obj;

    int GetId() const
    {
        return atoi(name.substr(name.length()-1).c_str());
    }

    bool operator<(const trigger &other) const
    {
        return GetId()<other.GetId();
    }
};

namespace sg
{
class sg_graphics_manager;
typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;

class isg_render_queue;
typedef std::shared_ptr<isg_render_queue> sg_render_queue_ptr;

class SGScenegraph
{
public:
    SGScenegraph(Logger * l, timer_ptr app_timer);
    virtual ~SGScenegraph();
    void clear();

    sg_object_ptr add_object(sg_object_ptr object);
    sg_light_object_ptr add_light_object();
    sg_mesh_object_ptr load_mesh_object (std::string file, bool load_textures);
    sg_skybox_object_ptr load_skybox    (std::string file, bool load_textures);

    sg_graphics_manager_ptr get_graphics_manager();
    timer_ptr get_timer();
    Logger * GetLogger();

    sg_camera_object_ptr get_active_camera();
    void set_active_camera(sg_camera_object_ptr cam);
    sg_render_queue_ptr get_render_queue();
    const sg_shared_mat_vars & get_shared_mat_vars() const;

    void set_override_material(sg_material_ptr material);
    sg_material_ptr get_override_material();

    virtual bool validate_transforms();
    virtual bool register_objects_for_rendering();
    void render_all();
    void update_all(float delta_time);

    void add_trigger(sg_empty_object_ptr obj,std::string name)
    {
        trigger a;
        a.obj=obj;
        a.name=name;
        m_triggers.push_back(a);
    }
    std::vector<trigger> get_triggers(){return m_triggers;}
    trigger get_trigger(std::string name)
    {
        for(trigger trig:m_triggers)
        {
            if(trig.name==name)
                return trig;
        }
        trigger a;
        return a;
    }

    sg_mesh_object_ptr get_mesh_obj(const std::string &name)
    {
        for(sg_object_ptr op:m_objects)
        {
            if(op->GetName()==name)
            {
                return std::static_pointer_cast<sg_mesh_object>(op);
            }
        }
        return nullptr;
    }
   sg_object_ptr object_depth_pick(int32_t x, int32_t y, int32_t w, int32_t h);
   glm::vec3 window_coords_to_world(float depth, int32_t x, int32_t y, int32_t w, int32_t h);

protected:
    void pre_render();
    void post_render();

    timer_ptr                   _timer;
    Logger*                     _logger;
    sg_render_queue_ptr         _renderQueue;
    sg_graphics_manager_ptr     _graphicsManager;
    sg_material_ptr             _currentMaterial,
                                m_override_material;
    sg_camera_object_ptr        m_active_camera;
    sg_shared_mat_vars          m_shared_mat_vars;

    std::vector<sg_object_ptr> m_objects;
    std::vector<trigger> m_triggers;
};

}

#endif // SCENEGRAPH_H
