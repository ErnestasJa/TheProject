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
    sg_material(uint32_t mat_type, shader_ptr mat_shader);
    virtual ~sg_material(){}

    virtual void set()=0;

    template <class T>
    void bind_var(sg_mvar<T> & mvar);

    ///-------------------------------------
    uint32_t    mat_type;
    shader_ptr  mat_shader;
};

struct sg_abstract_material: public sg_material
{
    sg_abstract_material(shader_ptr mat_shader);
    virtual ~sg_abstract_material(){}

    virtual void set();
    void load_and_bind_vars();

    virtual texture_ptr get_texture(uint32_t index);
    virtual int32_t     get_int(const std::string & name);
    virtual float       get_float(const std::string & name);
    virtual glm::vec3   get_vec3f(const std::string & name);
    virtual glm::mat4   get_mat4(const std::string & name);
    virtual glm::mat3   get_mat3(const std::string & name);

    virtual void        set_texture(uint32_t value, texture_ptr tex);
    virtual void        set_int(const std::string & name, int32_t value);
    virtual void        set_float(const std::string & name, float value);
    virtual void        set_vec3f(const std::string & name, glm::vec3 value);
    virtual void        set_mat4(const std::string & name, glm::mat4 value);
    virtual void        set_mat3(const std::string & name, glm::mat3 value);

    ///-------------------------
    std::vector< texture_ptr >          textures;
    std::vector< sg_mvar<int32_t> >     int_attr;
    std::vector< sg_mvar<float> >       float_attr;
    std::vector< sg_mvar<glm::vec3> >   vec3f_attr;
    std::vector< sg_mvar<glm::mat4> >   mat4_attr;
    std::vector< sg_mvar<glm::mat3> >   mat3_attr;
};

struct sg_material_static_mesh: public sg_material
{
    sg_material_static_mesh(shader_ptr mat_shader);
    virtual ~sg_material_static_mesh(){}

    virtual void set();
    virtual void set(sg_material_static_mesh * last); ///if we know last material was the same, maybe we can avoid doing extra work

    ///-------------------------------------
    sg_mvar<glm::mat4x4> mvp, m;
    sg_mvar<glm::mat3x3> n;
    sg_mvar<glm::vec3> light_pos, camera_pos;

    texture_ptr mat_texture;
};

struct sg_material_vsm_first_pass: public sg_material
{
    sg_material_vsm_first_pass(shader_ptr mat_shader);
    virtual ~sg_material_vsm_first_pass(){}

    virtual void set();

    ///-------------------------------------
    sg_mvar<glm::mat4x4> mvp;
};

struct sg_material_vsm_final_pass: public sg_material
{
    sg_material_vsm_final_pass(shader_ptr mat_shader);
    virtual ~sg_material_vsm_final_pass(){}


    virtual void set();

    ///-------------------------------------
    sg_mvar<glm::mat4x4> mvp, mv, m, s;
    sg_mvar<glm::mat3x3> n;
    sg_mvar<glm::vec3> light_pos, camera_pos;
    texture_ptr texture0;
    texture_ptr texture1;
};

struct sg_material_texture_filter: public sg_material
{
    sg_material_texture_filter(shader_ptr mat_shader);
    virtual ~sg_material_texture_filter(){}

    virtual void set();

    ///-------------------------------------
    texture_ptr texture0;
};

struct sg_material_point_sprite: public sg_material
{
    sg_material_point_sprite(shader_ptr mat_shader);
    virtual ~sg_material_point_sprite(){}

    virtual void set();

    ///-------------------------------------
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
