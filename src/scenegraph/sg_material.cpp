#include "precomp.h"
#include "sg_material.h"
#include "scenegraph.h"
#include "opengl/shader.h"

namespace sg
{

sg_material::sg_material()
{
}

///----------------------------------------------------------
binding sg_material_static_mesh::bindings[]=
{
    {"texture0",-1},
    {"mvp",-1},
    {"m",-1},
    {"n",-1},
    {"light_pos",-1},
    {"camera_pos",-1},
    {"",-1}
};

sg_material_static_mesh::sg_material_static_mesh()
{
    mat_type = SGM_STATIC_MESH;

    mvp.name="mvp";
    m.name="m";
    n.name="n";
    light_pos.name="light_pos";
    camera_pos.name="camera_pos";
}

void sg_material_static_mesh::set()
{
    mat_shader->set();

    mat_texture->set(0);
    mvp.set(mat_shader->bindings[1].index);
    m.set(mat_shader->bindings[2].index);
    n.set(mat_shader->bindings[3].index);
    light_pos.set(mat_shader->bindings[4].index);
    camera_pos.set(mat_shader->bindings[5].index);
}

void sg_material_static_mesh::set(sg_material_static_mesh * last)
{
    if(last->mat_texture!=mat_texture)
        mat_texture->set(0);

    if(!helpers::equals(last->mvp.value, mvp.value))
        mvp.set(mat_shader->bindings[1].index);

    if(!helpers::equals(last->m.value, m.value))
        m.set(mat_shader->bindings[2].index);

    if(!helpers::equals(last->n.value, n.value))
        n.set(mat_shader->bindings[3].index);

    ///these should not even need chaning if last material set those.
    if(!helpers::equals(last->light_pos.value, light_pos.value))
        light_pos.set(mat_shader->bindings[4].index);

    if(!helpers::equals(last->camera_pos.value, camera_pos.value))
        camera_pos.set(mat_shader->bindings[5].index);
}

///----------------------------------------------------------
binding sg_material_vsm_first_pass::bindings[]=
{
    {"mvp",-1},
    {"",-1}
};

sg_material_vsm_first_pass::sg_material_vsm_first_pass()
{
    mat_type = SGM_VSM_FIRST_PASS;
    mvp.name="mvp";
}

void sg_material_vsm_first_pass::set()
{
    mat_shader->set();
    mvp.set(mat_shader->bindings[0].index);
}

///----------------------------------------------------------
// FIXME (serengeor#1#): static bindings bad if multiple shaders use same material, then can't get locations for secondary shaders.

binding sg_material_texture_filter::bindings[]=
{
    {"texture0",-1},
    {"",-1}
};

sg_material_texture_filter::sg_material_texture_filter()
{
    mat_type = SGM_TEXTURE_FILTER;
}

void sg_material_texture_filter::set()
{
    mat_shader->set();
    texture0->set(0);
}

///----------------------------------------------------------
binding sg_material_vsm_final_pass::bindings[]=
{
    {"texture0",-1},
    {"m",-1},
    {"mvp",-1},
    {"s",-1},
    {"n",-1},
    {"light_pos",-1},
    {"camera_pos",-1},
    {"",-1}
};

sg_material_vsm_final_pass::sg_material_vsm_final_pass()
{
    mat_type = SGM_VSM_FINAL_PASS;

    m.name="m";
    //mv.name="mv";
    mvp.name="mvp";
    s.name="s";
    n.name="n";
    light_pos.name="light_pos";
    camera_pos.name="camera_pos";
}

void sg_material_vsm_final_pass::set()
{
    mat_shader->set();
    m.set(mat_shader->bindings[1].index);
    mvp.set(mat_shader->bindings[2].index);
    s.set(mat_shader->bindings[3].index);
    //mv.set(bindings[2].index);
    n.set(mat_shader->bindings[4].index);
    light_pos.set(mat_shader->bindings[5].index);
    camera_pos.set(mat_shader->bindings[6].index);
    texture0->set(0);
    //texture1->set(1);
}

///----------------------------------------------------------
binding sg_material_point_sprite::bindings[]=
{
    {"texture0",-1},
    {"vp",-1},
    {"cam_up",-1},
    {"cam_right",-1},
    {"bpos",-1},
    {"size",-1},
    {"",-1}
};

sg_material_point_sprite::sg_material_point_sprite()
{
    mat_type = SGM_POINT_SPRITE;
    vp.name="vp";
    cam_up.name="cam_up";
    cam_right.name="cam_right";
    pos.name="bpos";
    size.name="size";
    size=glm::vec3(1,1,1);
}

void sg_material_point_sprite::set()
{
    mat_shader->set();
    mat_texture->set(0);
    vp.set(mat_shader->bindings[1].index);
    cam_up.set(mat_shader->bindings[2].index);
    cam_right.set(mat_shader->bindings[3].index);
    pos.set(mat_shader->bindings[4].index);
    size.set(mat_shader->bindings[5].index);

}

}
