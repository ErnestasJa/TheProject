#ifndef SG_MESH_OBJECT_H
#define SG_MESH_OBJECT_H

#include "isg_object.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{

class sg_mesh_object: public isg_object
{
protected:
    mesh_ptr m_mesh;

public:
    sg_mesh_object(mesh_ptr ptr);
    virtual ~sg_mesh_object();

    virtual uint32_t get_type();
    virtual void render();
};

typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

}

#endif // SG_MESH_OBJECT_H
