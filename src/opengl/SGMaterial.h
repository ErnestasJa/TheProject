#ifndef SG_MATERIAL_H
#define SG_MATERIAL_H

// TODO (Ernestas#1#): figure this out ...
//
#include "opengl/Shader.h"
#include "opengl/Texture.h"
#include "SGMVar.h"

namespace sg
{

class SGScenegraph;

struct sg_material
{
    sg_material(shader_ptr mat_shader, uint32_t Id = 0);
    virtual ~sg_material(){}

    virtual void set();

    template <class T> void bind_var(sg_mvar<T> & mvar);
    void load_and_bind_vars();

    virtual Texture_ptr get_texture(uint32_t index);
    virtual int32_t     get_int(const std::string & name);
    virtual float       get_float(const std::string & name);
    virtual glm::vec3   get_vec3f(const std::string & name);
    virtual glm::mat4   get_mat4(const std::string & name);
    virtual glm::mat3   get_mat3(const std::string & name);

    virtual void        set_texture(uint32_t value, Texture_ptr tex);
    virtual void        set_int(const std::string & name, int32_t value);
    virtual void        set_float(const std::string & name, float value);
    virtual void        set_vec3f(const std::string & name, glm::vec3 value);
    virtual void        set_mat4(const std::string & name, glm::mat4 value);
    virtual void        set_mat3(const std::string & name, glm::mat3 value);

    ///-------------------------
    std::vector< Texture_ptr >          textures;
    std::vector< sg_mvar<int32_t> >     int_attr;
    std::vector< sg_mvar<float> >       float_attr;
    std::vector< sg_mvar<glm::vec3> >   vec3f_attr;
    std::vector< sg_mvar<glm::mat4> >   mat4_attr;
    std::vector< sg_mvar<glm::mat3> >   mat3_attr;

    shader_ptr  mat_shader;
    uint32_t    mat_id;
};

typedef std::shared_ptr<sg_material> sg_material_ptr;

}

#endif // SG_MATERIAL_H
