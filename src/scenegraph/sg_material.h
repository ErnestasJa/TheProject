#ifndef SG_MATERIAL_H
#define SG_MATERIAL_H

// TODO (Ernestas#1#): figure this out ...
//
#include "opengl/shader.h"
#include "opengl/texture.h"
#include "scenegraph/sg_material_types.h"
#include "scenegraph/sg_mvar.h"

namespace sg
{
// TODO (Ernestas#1#): add some matrix mgr in smgr and add custom uniform setting for each sgo or sth
// TODO (Ernestas#1#): make light data available to materials __if they need it__.
class scenegraph;
struct sg_material
{
    sg_material();
    virtual ~sg_material(){}

    virtual void set()=0;

    ///-------------------------------------
    uint32_t    mat_type;
    shader_ptr  mat_shader;
};

struct sg_material_static_mesh: public sg_material
{
    sg_material_static_mesh();
    virtual ~sg_material_static_mesh(){}

    virtual void set();
    virtual void set(sg_material_static_mesh * last); ///if we know last material was the same, maybe we can avoid doing extra work

    ///-------------------------------------
    static binding bindings[];

    sg_mvar<glm::mat4x4> mvp, m;
    sg_mvar<glm::mat3x3> n;
    sg_mvar<glm::vec3> light_pos, camera_pos;

    texture_ptr mat_texture;
};

struct sg_material_vsm_first_pass: public sg_material
{
    sg_material_vsm_first_pass();
    virtual ~sg_material_vsm_first_pass(){}

    virtual void set();

    ///-------------------------------------
    static binding bindings[];
    sg_mvar<glm::mat4x4> mvp;
};

struct sg_material_vsm_final_pass: public sg_material
{
    sg_material_vsm_final_pass();
    virtual ~sg_material_vsm_final_pass(){}


    virtual void set();

    ///-------------------------------------
    static binding bindings[];
    sg_mvar<glm::mat4x4> mvp, mv, m, s;
    sg_mvar<glm::mat3x3> n;
    sg_mvar<glm::vec3> light_pos, camera_pos;
    texture_ptr texture0;
    texture_ptr texture1;
};

struct sg_material_texture_filter: public sg_material
{
    sg_material_texture_filter();
    virtual ~sg_material_texture_filter(){}

    virtual void set();

    ///-------------------------------------
    static binding bindings[];
    texture_ptr texture0;
};

struct sg_material_point_sprite: public sg_material
{
    sg_material_point_sprite();
    virtual ~sg_material_point_sprite(){}

    virtual void set();

    ///-------------------------------------
    static binding bindings[];
    sg_mvar<glm::mat4x4> vp;
    sg_mvar<glm::vec3> cam_up, cam_right, pos, size;
    texture_ptr mat_texture;
};

typedef std::shared_ptr<sg_material> sg_material_ptr;
typedef std::shared_ptr<sg_material_static_mesh> sg_material_static_mesh_ptr;
typedef std::shared_ptr<sg_material_point_sprite> sg_material_point_sprite_ptr;
typedef std::shared_ptr<sg_material_vsm_first_pass> sg_material_vsm_first_pass_ptr;
typedef std::shared_ptr<sg_material_vsm_final_pass> sg_material_vsm_final_pass_ptr;

}

#endif // SG_MATERIAL_H
