#ifndef SG_MVAR_H
#define SG_MVAR_H

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

template <class T>
struct MVar
{
    MVar();
    MVar(int32_t binding_index, std::string name, T val);
    MVar(int32_t binding_index, std::string name);
    MVar(std::string name);

    void Set() const;
    void Set(int32_t binding_index) const;

    MVar<T> & operator = (const T & val);
    operator T () { return value; }

///-------------------------------
    std::string name;
    T value;
    uint32_t count;
    int32_t binding_index;
};

template <class T>
MVar<T>::MVar():count(1), binding_index(-1){}

template <class T>
MVar<T>::MVar(int32_t binding_index, std::string name, T val){this->binding_index = binding_index; this->name=name; this->value=val;}

template <class T>
MVar<T>::MVar(int32_t binding_index, std::string name){this->binding_index = binding_index; this->name=name;}

template <class T>
MVar<T>::MVar(std::string name){this->binding_index = -1; this->name=name;}


template <>
void MVar<float>::Set() const;

template <>
void MVar<int32_t>::Set() const;

template <>
void MVar<glm::mat4x4>::Set() const;

template <>
void MVar<glm::mat3x3>::Set() const;

template <>
void MVar<glm::vec3>::Set() const;

template <class T>
MVar<T> & MVar<T>::operator = (const T & val)
{
    value = val;
    return *this;
}

#endif // SG_MVAR_H
