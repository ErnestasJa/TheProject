#ifndef SG_AABB_WIREFRAME_OBJECT_H
#define SG_AABB_WIREFRAME_OBJECT_H


#include "ISGObject.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{
class sg_mesh_object; typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

class sg_aabb_wireframe_object: public isg_object
{
public:
    sg_aabb_wireframe_object(SGScenegraph * sg, sg_mesh_object_ptr mobj);
    virtual ~sg_aabb_wireframe_object();

    virtual uint32_t get_type();

    virtual void render(SGScenegraph * sg);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual const glm::mat4x4 & get_absolute_transform();

protected:
    sg_mesh_object_ptr m_obj;
    mesh_ptr m_mesh;
    sg_material_ptr m_material;
};

typedef std::shared_ptr<sg_aabb_wireframe_object> sg_aabb_wireframe_object_ptr;

}

#endif // SG_AABB_WIREFRAME_OBJECT_H
