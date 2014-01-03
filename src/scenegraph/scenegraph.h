#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "isg_object.h"

namespace sg
{

class scenegraph
{
public:
    scenegraph();
    virtual ~scenegraph();

    virtual void add_object(sg_object_ptr obj);
    virtual void render_all();

protected:

    virtual void pre_render();
    virtual void post_render();

    std::vector<sg_object_ptr> m_objects;
};

}

#endif // SCENEGRAPH_H
