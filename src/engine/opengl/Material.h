#ifndef MATERIAL_H
#define MATERIAL_H

#include "opengl/Shader.h"
#include "opengl/Texture.h"
#include "MVar.h"

struct Material
{
    Material(ShaderPtr mat_shader, uint32_t Id = 0);
    virtual ~Material(){}

    virtual void Set();

    template <class T> void BindVar(MVar<T> & mvar);
    void LoadAndBindVars();

    virtual ShaderPtr   GetShader();
    virtual TexturePtr  GetTexture(uint32_t index);
    virtual int32_t     GetInt(const std::string & name);
    virtual float       GetFloat(const std::string & name);
    virtual glm::vec3   GetVec3f(const std::string & name);
    virtual glm::mat4   GetMat4(const std::string & name);
    virtual glm::mat3   GetMat3(const std::string & name);

    virtual void        SetTexture(uint32_t value, TexturePtr tex);
    virtual void        SetInt(const std::string & name, int32_t value);
    virtual void        SetFloat(const std::string & name, float value);
    virtual void        SetVec3f(const std::string & name, glm::vec3 value);
    virtual void        SetMat4(const std::string & name, glm::mat4 value);
    virtual void        SetMat3(const std::string & name, glm::mat3 value);

    ///-------------------------
    vector< TexturePtr >       textures;
    vector< MVar<int32_t> >     int_attr;
    vector< MVar<float> >       float_attr;
    vector< MVar<glm::vec3> >   vec3f_attr;
    vector< MVar<glm::mat4> >   mat4_attr;
    vector< MVar<glm::mat3> >   mat3_attr;

private:
    ShaderPtr   m_shader;
    uint32_t    m_id;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif // MATERIAL_H
