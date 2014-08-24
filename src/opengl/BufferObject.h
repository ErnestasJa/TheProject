#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "IBufferObject.h"

template <class T>
struct BufferObject: public IBufferObject
{
    std::vector<T> data;

    //void Init_buffers();
    virtual void Init();
    virtual void Upload();

    virtual IBufferObject::IBO_TYPE GetType(){return IBufferObject::DATA;}
    virtual uint32_t GetDataType();
    virtual uint32_t GetComponentCount();
    virtual uint32_t GetSize(){return data.size();}
};

template <class T>
struct IndexBufferObject: public IBufferObject
{
    std::vector<T> data;

    //void Init_buffers();
    virtual void Init();
    virtual void Upload();

    virtual IBufferObject::IBO_TYPE GetType(){return IBufferObject::INDEX;}
    virtual uint32_t GetDataType();
    virtual uint32_t GetComponentCount();
    virtual uint32_t GetSize(){return data.size();}
};

#endif // BUFFER_OBJECT_H
