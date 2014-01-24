#include "precomp.h"
#include "sg_material.h"

namespace sg
{

bool sg_material::operator == (const sg_material & m)
{
    if(m.mat_shader != mat_shader)
        return false;

    loopi(8)
    if(mat_textures[i]!=m.mat_textures[i])
        return false;

    return true;
}

bool sg_material::operator != (const sg_material & m)
{
    return !(*this == m);
}

void sg_material::set()
{
    mat_shader->set();

    loopi(8)
    {
        if(mat_textures[i])
        {
            mat_textures[i]->set(i);
        }
    }
}

}
