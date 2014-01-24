#ifndef SG_MATERIAL_H
#define SG_MATERIAL_H

// TODO (Ernestas#1#): figure this out ...
//
#include "opengl/shader.h"
#include "opengl/texture.h"

namespace sg
{

struct sg_material
{
    shader_ptr mat_shader;
    texture_ptr mat_textures[8];

    void set();

    bool operator != (const sg_material & m);
    bool operator == (const sg_material & m);
};

}

#endif // SG_MATERIAL_H
