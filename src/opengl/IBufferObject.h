#ifndef IBUFFER_OBJECT_H
#define IBUFFER_OBJECT_H

struct ibuffer_object
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

    uint32_t id;
    uint32_t usage_hint;
    //bool enabled;

    ibuffer_object();
    virtual ~ibuffer_object();

    virtual void init() = 0;
    virtual void upload() = 0;

    virtual ibuffer_object::IBO_TYPE get_type() = 0;
    virtual uint32_t get_data_type() = 0;
    virtual uint32_t get_component_count() = 0;
    virtual uint32_t get_size() = 0;

};

#endif // IBUFFER_OBJECT_H
