#ifndef IBUFFER_OBJECT_H
#define IBUFFER_OBJECT_H

struct IBufferObject
{
    enum IBO_TYPE
    {
        DATA = 0,
        INDEX
    };

    enum USAGE_HINT
    {
        STATIC = 0,
        DYNAMIC,
        STREAM
    };

    uint32_t Id;
    uint32_t usage_hint;
    //bool enabled;

    IBufferObject();
    virtual ~IBufferObject();

    virtual void init() = 0;
    virtual void upload() = 0;

    virtual IBufferObject::IBO_TYPE get_type() = 0;
    virtual uint32_t get_data_type() = 0;
    virtual uint32_t get_component_count() = 0;
    virtual uint32_t get_size() = 0;

};

#endif // IBUFFER_OBJECT_H
