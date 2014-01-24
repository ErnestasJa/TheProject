#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "sg_objects.h"
#include "sg_material.h"

namespace sg
{

class scenegraph
{
public:
    scenegraph();
    virtual ~scenegraph();

    virtual void add_object(sg_object_ptr obj);
    virtual void render_all();

    sg_camera_object_ptr get_active_camera();
    void set_active_camera(sg_camera_object_ptr cam);

    void on_set_material(const sg_material & mat);

protected:
    virtual void pre_render();
    virtual void post_render();

    sg_camera_object_ptr        m_active_camera;
    std::vector<sg_object_ptr>  m_objects;
    sg_material                 m_current_material;

    glm::mat4 VP, MVP;
};

}

#endif // SCENEGRAPH_H
