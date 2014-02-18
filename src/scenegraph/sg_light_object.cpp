#include "precomp.h"
#include "sg_light_object.h"

namespace sg
{

sg_light_object::sg_light_object(scenegraph * sg): isg_object(sg)
{
    //ctor
}

sg_light_object::~sg_light_object()
{
    //dtor
}

void sg_light_object::render(scenegraph * sg)
{

}

sg_aabb sg_light_object::get_aabb()
{
    return m_aabb;
}

uint32_t sg_light_object::get_type()
{
    return SGO_LIGHT;
}

sg_material_ptr sg_light_object::get_material(uint32_t index)
{
    return sg_material_ptr();
}

uint32_t sg_light_object::get_material_count()
{
    return 0;
}

void sg_light_object::recalculate_aabb()
{

}
}
