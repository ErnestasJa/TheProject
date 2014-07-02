#pragma once

#include "isg_object.h"
#include "opengl/buffer_object.h"

namespace sg
{

class sg_empty_object: public isg_object
{
public:
    sg_empty_object(scenegraph * sg):isg_object(sg){};
    virtual ~sg_empty_object(){};

    uint32_t get_type(){return 0;}
    void render(sg::scenegraph* sg){}
    sg_material_ptr get_material(uint32_t index){return nullptr;}
    bool set_material(uint32_t index,sg_material_ptr mat){return false;}
    uint32_t get_material_count(){return 0;}

protected:
};

typedef std::shared_ptr<sg_empty_object> sg_empty_object_ptr;

}
