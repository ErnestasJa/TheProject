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
    sg_mvar(int32_t binding_index, std::string name, T val);
    sg_mvar(int32_t binding_index, std::string name);
    sg_mvar(std::string name);

    void set() const;
    void set(int32_t binding_index) const;

    T get();
    const T & get() const;


    sg_mvar<T> & operator = (const T & val);

///-------------------------------
    std::string name;
    T value;
    uint32_t count;
    int32_t binding_index;
};

template <class T>
sg_mvar<T>::sg_mvar():count(1), binding_index(-1){}

template <class T>
sg_mvar<T>::sg_mvar(int32_t binding_index, std::string name, T val){this->binding_index = binding_index; this->name=name; this->value=val;}

template <class T>
sg_mvar<T>::sg_mvar(int32_t binding_index, std::string name){this->binding_index = binding_index; this->name=name;}

template <class T>
sg_mvar<T>::sg_mvar(std::string name){this->binding_index = -1; this->name=name;}


template <>
void sg_mvar<float>::set() const;

template <>
void sg_mvar<int32_t>::set() const;

template <>
void sg_mvar<glm::mat4x4>::set() const;

template <>
void sg_mvar<glm::mat3x3>::set() const;

template <>
void sg_mvar<glm::vec3>::set() const;

template <class T>
sg_mvar<T> & sg_mvar<T>::operator = (const T & val)
{
    value = val;
    return *this;
}

}

#endif // SG_MVAR_H
