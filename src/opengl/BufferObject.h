#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "IBufferObject.h"

template <class T>
struct BufferObject: public IBufferObject
{
    std::vector<T> data;

    //void init_buffers();
    virtual void init();
    virtual void upload();

    virtual IBufferObject::IBO_TYPE get_type(){return IBufferObject::DATA;}
    virtual uint32_t get_data_type();
    virtual uint32_t get_component_count();
    virtual uint32_t get_size(){return data.size();}
};

template <class T>
struct IndexBufferObject: public IBufferObject
{
    std::vector<T> data;

    //void init_buffers();
    virtual void init();
    virtual void upload();

    virtual IBufferObject::IBO_TYPE get_type(){return IBufferObject::INDEX;}
    virtual uint32_t get_data_type();
    virtual uint32_t get_component_count();
    virtual uint32_t get_size(){return data.size();}
};

#endif // BUFFER_OBJECT_H
