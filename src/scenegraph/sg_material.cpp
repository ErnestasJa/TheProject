#include "precomp.h"
#include "sg_material.h"
#include "scenegraph.h"
#include "opengl/shader.h"

namespace sg
{

sg_material::sg_material(uint32_t type, shader_ptr shader)
{
    this->mat_type = type;
    this->mat_shader = shader;
}

template <class T>
void sg_material::bind_var(sg_mvar<T> & mvar)
{
    mvar.binding_index = mat_shader->get_binding(mvar.name).index;
    if(mvar.binding_index==-1)
        throw mvar.binding_index;
}

///----------------------------------------------------------
sg_abstract_material::sg_abstract_material(shader_ptr mat_shader):sg_material(SGM_ABSTRACT_MATERIAL,mat_shader)
{
    load_and_bind_vars();
}

void sg_abstract_material::set()
{
    mat_shader->set();

    for(auto val :int_attr)
        val.set();
    for(auto val :float_attr)
        val.set();
    for(auto val :vec3f_attr)
        val.set();
    for(auto val :mat3_attr)
        val.set();
    for(auto val :mat4_attr)
        val.set();
}

void sg_abstract_material::load_and_bind_vars()
{
    int_attr.clear();
    float_attr.clear();
    vec3f_attr.clear();
    mat4_attr.clear();
    mat3_attr.clear();

    for(binding & t: mat_shader->bindings)
    {
        switch(t.type)
        {
        case GL_INT:
            int_attr.push_back(sg_mvar<int32_t>(t.index,t.name));
            break;
        case GL_FLOAT:
            float_attr.push_back(sg_mvar<float>(t.index,t.name));
            break;
        case GL_FLOAT_VEC3:
            vec3f_attr.push_back(sg_mvar<glm::vec3>(t.index,t.name));
            break;
        case GL_FLOAT_MAT4:
            mat4_attr.push_back(sg_mvar<glm::mat4>(t.index,t.name));
            break;
        case GL_FLOAT_MAT3:
            mat3_attr.push_back(sg_mvar<glm::mat3>(t.index,t.name));
            break;
        }
    }
}

#define get_attr(x,y) for(x attr: y) {if(attr.name==name) return attr.value;} return x().value
int32_t sg_abstract_material::get_int(const std::string & name)
{
    get_attr(sg_mvar<int32_t>,int_attr);
}

float sg_abstract_material::get_float(const std::string & name)
{
    get_attr(sg_mvar<float>,float_attr);
}

glm::vec3 sg_abstract_material::get_vec3f(const std::string & name)
{
    get_attr(sg_mvar<glm::vec3>,vec3f_attr);
}

glm::mat4 sg_abstract_material::get_mat4(const std::string & name)
{
    get_attr(sg_mvar<glm::mat4>,mat4_attr);
}

glm::mat3 sg_abstract_material::get_mat3(const std::string & name)
{
    get_attr(sg_mvar<glm::mat3>,mat3_attr);
}

#undef get_attr
#define set_attr(x,y) for(x attr: y) {if(attr.name==name) { attr.value = value;}}
//#define set_attr(x,y) for(x attr: y) {if(attr.name==name) { attr.value = value; attr.set(mat_shader->get_binding(name).index);}}

void sg_abstract_material::set_int(const std::string & name, int32_t value)
{
    set_attr(sg_mvar<int32_t>,int_attr);
}

void sg_abstract_material::set_float(const std::string & name, float value)
{
    set_attr(sg_mvar<float>,float_attr);
}

void sg_abstract_material::set_vec3f(const std::string & name, glm::vec3 value)
{
    set_attr(sg_mvar<glm::vec3>,vec3f_attr);
}

void sg_abstract_material::set_mat4(const std::string & name, glm::mat4 value)
{
    set_attr(sg_mvar<glm::mat4>,mat4_attr);
}

void sg_abstract_material::set_mat3(const std::string & name, glm::mat3 value)
{
    set_attr(sg_mvar<glm::mat3>,mat3_attr);
}
#undef set_attr

///----------------------------------------------------------
sg_material_static_mesh::sg_material_static_mesh(shader_ptr shader):sg_material(SGM_STATIC_MESH,shader)
{
    mvp.name="mvp";
    m.name="m";
    n.name="n";
    light_pos.name="light_pos";
    camera_pos.name="camera_pos";

    bind_var(mvp);
    bind_var(m);
    bind_var(n);
    bind_var(light_pos);
    bind_var(camera_pos);
}

void sg_material_static_mesh::set()
{
    mat_shader->set();

    mat_texture->set(0);
    mvp.set();
    m.set();
    n.set();
    light_pos.set();
    camera_pos.set();
}

void sg_material_static_mesh::set(sg_material_static_mesh * last)
{
    if(last->mat_texture!=mat_texture)
        mat_texture->set(0);

    if(!helpers::equals(last->mvp.value, mvp.value))
        mvp.set();

    if(!helpers::equals(last->m.value, m.value))
        m.set();

    if(!helpers::equals(last->n.value, n.value))
        n.set();

    ///these should not even need chaning if last material set those.
    if(!helpers::equals(last->light_pos.value, light_pos.value))
        light_pos.set();

    if(!helpers::equals(last->camera_pos.value, camera_pos.value))
        camera_pos.set();
}

///----------------------------------------------------------

sg_material_vsm_first_pass::sg_material_vsm_first_pass(shader_ptr shader):sg_material(SGM_VSM_FIRST_PASS,shader)
{
    mat_type = SGM_VSM_FIRST_PASS;
    mvp.name="mvp";
    bind_var(mvp);
}

void sg_material_vsm_first_pass::set()
{
    mat_shader->set();
    mvp.set();
}

///----------------------------------------------------------
// FIXME (serengeor#1#): static bindings bad if multiple shaders use same material, then can't get locations for secondary shaders.

sg_material_texture_filter::sg_material_texture_filter(shader_ptr mat_shader):sg_material(SGM_TEXTURE_FILTER,mat_shader)
{
    mat_type = SGM_TEXTURE_FILTER;
}

void sg_material_texture_filter::set()
{
    mat_shader->set();
    texture0->set(0);
}

///----------------------------------------------------------

sg_material_vsm_final_pass::sg_material_vsm_final_pass(shader_ptr mat_shader):sg_material(SGM_VSM_FINAL_PASS,mat_shader)
{
    mat_type = SGM_VSM_FINAL_PASS;

    m.name="m";
    //mv.name="mv";
    mvp.name="mvp";
    s.name="s";
    n.name="n";
    light_pos.name="light_pos";
    camera_pos.name="camera_pos";

    bind_var(m);
    bind_var(mvp);
    bind_var(s);
    bind_var(n);
    bind_var(light_pos);
    bind_var(camera_pos);
}

void sg_material_vsm_final_pass::set()
{
    mat_shader->set();
    m.set();
    mvp.set();
    s.set();
    //mv.set(bindings[2].index);
    n.set();
    light_pos.set();
    camera_pos.set();
    texture0->set(0);
    texture1->set(1);
}

///----------------------------------------------------------

sg_material_point_sprite::sg_material_point_sprite(shader_ptr mat_shader):sg_material(SGM_POINT_SPRITE,mat_shader)
{
    mat_type = SGM_POINT_SPRITE;
    vp.name="vp";
    cam_up.name="cam_up";
    cam_right.name="cam_right";
    pos.name="bpos";
    size.name="size";
    size=glm::vec3(1,1,1);

    bind_var(vp);
    bind_var(cam_up);
    bind_var(cam_right);
    bind_var(pos);
    bind_var(size);
}

void sg_material_point_sprite::set()
{
    mat_shader->set();
    mat_texture->set(0);
    vp.set();
    cam_up.set();
    cam_right.set();
    pos.set();
    size.set();

}

}
