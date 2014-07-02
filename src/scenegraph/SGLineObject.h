#ifndef SG_AABB_WIREFRAME_OBJECT_H
#define SG_AABB_WIREFRAME_OBJECT_H


#include "ISGObject.h"
#include "opengl/BufferObject.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{

class sg_line_object: public isg_object
{
public:
    sg_line_object(scenegraph * sg);
    virtual ~sg_line_object();

    virtual uint32_t get_type();

    virtual void render(scenegraph * sg);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual sg_abstract_material * get_abstract_material();
    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual uint32_t get_material_count();

    virtual void add_segment(glm::vec3 p0, glm::vec3 p1);
    virtual void clear_segments();
    virtual void update_mesh();

protected:
    mesh_ptr m_mesh;
    sg_material_ptr m_material;
    sg_abstract_material * m_abstract_material;

    buffer_object<glm::vec3> * verts;
};

typedef std::shared_ptr<sg_line_object> sg_line_object_ptr;

}

#endif // SG_AABB_WIREFRAME_OBJECT_H
