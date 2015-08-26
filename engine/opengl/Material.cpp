#include "Precomp.h"
#include "Material.h"
#include "opengl/Shader.h"


template <class T>
void Material::BindVar(MVar<T> & mvar)
{
    mvar.binding_index = m_shader->GetBinding(mvar.name).index;
    if(mvar.binding_index==-1)
        throw mvar.binding_index;
}

Material::Material(ShaderPtr mat_shader, uint32_t mat_id)
{
    this->m_id = mat_id;
    this->m_shader = mat_shader;
    textures.resize(8);
    LoadAndBindVars();
}

void Material::Set()
{
    m_shader->Set();

    loop(i,8)
    if(textures[i])
        textures[i]->Set(i);

    for(auto val :int_attr)
        val.Set();
    for(auto val :float_attr)
        val.Set();
    for(auto val :vec3f_attr)
        val.Set();
    for(auto val :mat3_attr)
        val.Set();
    for(auto val :mat4_attr)
        val.Set();
}

void Material::LoadAndBindVars()
{
    int_attr.clear();
    float_attr.clear();
    vec3f_attr.clear();
    mat4_attr.clear();
    mat3_attr.clear();

    for(Binding & t: m_shader->bindings)
    {
        switch(t.Type)
        {
        case GL_INT:
        case GL_SAMPLER_2D:
            int_attr.push_back(MVar<int32_t>(t.index,t.name));
            break;
        case GL_FLOAT:
            float_attr.push_back(MVar<float>(t.index,t.name));
            break;
        case GL_FLOAT_VEC3:
            vec3f_attr.push_back(MVar<glm::vec3>(t.index,t.name));
            break;
        case GL_FLOAT_MAT4:
            mat4_attr.push_back(MVar<glm::mat4>(t.index,t.name));
            break;
        case GL_FLOAT_MAT3:
            mat3_attr.push_back(MVar<glm::mat3>(t.index,t.name));
            break;
        }
    }
}

ShaderPtr Material::GetShader()
{
    return m_shader;
}

TexturePtr Material::GetTexture(uint32_t index)
{
    if(index<textures.size())
        return textures[index];

    return TexturePtr();
}

#define get_attr(x,y) for(x attr: y) {if(attr.name==name) return attr.value;} return x().value

int32_t Material::GetInt(const std::string & name)
{
    get_attr(MVar<int32_t>,int_attr);
}

float Material::GetFloat(const std::string & name)
{
    get_attr(MVar<float>,float_attr);
}

glm::vec3 Material::GetVec3f(const std::string & name)
{
    get_attr(MVar<glm::vec3>,vec3f_attr);
}

glm::mat4 Material::GetMat4(const std::string & name)
{
    get_attr(MVar<glm::mat4>,mat4_attr);
}

glm::mat3 Material::GetMat3(const std::string & name)
{
    get_attr(MVar<glm::mat3>,mat3_attr);
}

#undef get_attr
#define set_attr(x,y) for(x & attr: y) {if(attr.name==name) { attr.value = value; return;}} assert(false);
//#define set_attr(x,y) for(x attr: y) {if(attr.name==name) { attr.value = value; attr.set(mat_shader->get_binding(name).index);}}

void Material::SetTexture(uint32_t index, TexturePtr tex)
{
    if(index<8)
        textures[index]=tex;
}

void Material::SetInt(const std::string & name, int32_t value)
{
    for(MVar<int32_t> & attr: int_attr)
    {
        if(attr.name==name) { attr.value = value; return;}
    }
    assert(false);
}

void Material::SetFloat(const std::string & name, float value)
{
    set_attr(MVar<float>,float_attr);
}

void Material::SetVec3f(const std::string & name, glm::vec3 value)
{
    set_attr(MVar<glm::vec3>,vec3f_attr);
}

void Material::SetMat4(const std::string & name, glm::mat4 value)
{
    set_attr(MVar<glm::mat4>,mat4_attr);
}

void Material::SetMat3(const std::string & name, glm::mat3 value)
{
    set_attr(MVar<glm::mat3>,mat3_attr);
}
#undef set_attr
