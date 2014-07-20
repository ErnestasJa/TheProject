#pragma once

#include "ISGObject.h"
#include "opengl/BufferObject.h"

namespace sg
{

class sg_empty_object: public isg_object
{
public:
    sg_empty_object(SGScenegraph * sg):isg_object(sg){};
    virtual ~sg_empty_object(){};

    uint32_t get_type(){return 0;}
    void Render(sg::SGScenegraph* sg){}
    sg_material_ptr get_material(uint32_t index){return nullptr;}
    bool set_material(uint32_t index,sg_material_ptr mat){return false;}
    uint32_t get_material_count(){return 0;}

protected:
};

typedef std::shared_ptr<sg_empty_object> sg_empty_object_ptr;

}
