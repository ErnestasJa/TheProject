#ifndef SG_MVAR_H
#define SG_MVAR_H

class shader;
typedef std::shared_ptr<shader> shader_ptr;

namespace sg
{

template <class T>
struct sg_mvar
{
    sg_mvar();
    sg_mvar(std::string name, T val);
    sg_mvar(std::string name);

    void set(int32_t binding_index) const;
    sg_mvar & operator = (const T & val);

///-------------------------------
    std::string name;
    T value;
    uint32_t count;
};

template <class T>
sg_mvar<T>::sg_mvar():count(1){}

template <class T>
sg_mvar<T>::sg_mvar(std::string name, T val){this->name=name; this->value=val;}

template <class T>
sg_mvar<T>::sg_mvar(std::string name){this->name=name;}

template <>
void sg_mvar<float>::set(int32_t binding_index) const;

template <>
void sg_mvar<int32_t>::set(int32_t binding_index) const;

template <>
void sg_mvar<glm::mat4x4>::set(int32_t binding_index) const;

template <>
void sg_mvar<glm::mat3x3>::set(int32_t binding_index) const;

template <>
void sg_mvar<glm::vec3>::set(int32_t binding_index) const;

template <class T>
sg_mvar<T> & sg_mvar<T>::operator = (const T & val)
{
    value = val;
    return *this;
}

}

#endif // SG_MVAR_H
