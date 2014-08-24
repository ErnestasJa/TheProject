#include "Precomp.h"
#include "SGMaterial.h"
#include "opengl/Shader.h"

namespace sg
{

template <class T>
void sg_material::bind_var(sg_mvar<T> & mvar)
{
    mvar.binding_index = mat_shader->get_binding(mvar.name).index;
    if(mvar.binding_index==-1)
        throw mvar.binding_index;
}

sg_material::sg_material(shader_ptr mat_shader, uint32_t mat_id)
{
    this->mat_id = mat_id;
    this->mat_shader = mat_shader;
    textures.resize(8);
    load_and_bind_vars();
}

void sg_material::set()
{
    mat_shader->set();

    loopi(8)
    if(textures[i])
        textures[i]->set(i);

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

void sg_material::load_and_bind_vars()
{
    int_attr.clear();
    float_attr.clear();
    vec3f_attr.clear();
    mat4_attr.clear();
    mat3_attr.clear();

    for(binding & t: mat_shader->bindings)
    {
        switch(t.Type)
        {
        case GL_INT:
        case GL_SAMPLER_2D:
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

Texture_ptr sg_material::get_texture(uint32_t index)
{
    if(index<textures.size())
        return textures[index];

    return Texture_ptr();
}

#define get_attr(x,y) for(x attr: y) {if(attr.name==name) return attr.value;} return x().value

int32_t sg_material::get_int(const std::string & name)
{
    get_attr(sg_mvar<int32_t>,int_attr);
}

float sg_material::get_float(const std::string & name)
{
    get_attr(sg_mvar<float>,float_attr);
}

glm::vec3 sg_material::get_vec3f(const std::string & name)
{
    get_attr(sg_mvar<glm::vec3>,vec3f_attr);
}

glm::mat4 sg_material::get_mat4(const std::string & name)
{
    get_attr(sg_mvar<glm::mat4>,mat4_attr);
}

glm::mat3 sg_material::get_mat3(const std::string & name)
{
    get_attr(sg_mvar<glm::mat3>,mat3_attr);
}

#undef get_attr
#define set_attr(x,y) for(x & attr: y) {if(attr.name==name) { attr.value = value; return;}} assert(false);
//#define set_attr(x,y) for(x attr: y) {if(attr.name==name) { attr.value = value; attr.set(mat_shader->get_binding(name).index);}}

void sg_material::set_texture(uint32_t index, Texture_ptr tex)
{
    if(index<8)
        textures[index]=tex;
}

void sg_material::set_int(const std::string & name, int32_t value)
{
    for(sg_mvar<int32_t> & attr: int_attr)
    {
        if(attr.name==name) { attr.value = value; return;}
    }
    assert(false);
}

void sg_material::set_float(const std::string & name, float value)
{
    set_attr(sg_mvar<float>,float_attr);
}

void sg_material::set_vec3f(const std::string & name, glm::vec3 value)
{
    set_attr(sg_mvar<glm::vec3>,vec3f_attr);
}

void sg_material::set_mat4(const std::string & name, glm::mat4 value)
{
    set_attr(sg_mvar<glm::mat4>,mat4_attr);
}

void sg_material::set_mat3(const std::string & name, glm::mat3 value)
{
    set_attr(sg_mvar<glm::mat3>,mat3_attr);
}
#undef set_attr

}
