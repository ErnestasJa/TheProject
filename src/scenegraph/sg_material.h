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
    ~sg_material();

    virtual void set(scenegraph * sg)=0;

    ///-------------------------------------
    uint32_t    mat_type;
    shader_ptr  mat_shader;
};



struct sg_material_static_mesh: public sg_material
{
    sg_material_static_mesh();

    virtual void set(scenegraph * sg);

    ///-------------------------------------
    sg_mvar<glm::mat4x4> mvp, mv,m;
    sg_mvar<glm::mat3x3> n;
    sg_mvar<glm::vec3> light_pos, camera_pos;

    texture_ptr mat_texture;
};

struct sg_material_point_sprite: public sg_material
{
    sg_material_point_sprite();

    virtual void set(scenegraph * sg);

    ///-------------------------------------
    sg_mvar<glm::mat4x4> vp;
    sg_mvar<glm::vec3> cam_up, cam_right, pos, size;
    texture_ptr mat_texture;
};

typedef std::shared_ptr<sg_material> sg_material_ptr;
typedef std::shared_ptr<sg_material_static_mesh> sg_material_static_mesh_ptr;
typedef std::shared_ptr<sg_material_point_sprite> sg_material_point_sprite_ptr;

}

#endif // SG_MATERIAL_H
