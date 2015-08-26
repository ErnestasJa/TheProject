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
    uint32_t UsageHint;
    //bool enabled;

    IBufferObject();
    virtual ~IBufferObject();

    virtual void Init() = 0;
    virtual void Upload() = 0;

    virtual IBufferObject::IBO_TYPE GetType() = 0;
    virtual uint32_t GetDataType() = 0;
    virtual uint32_t GetComponentCount() = 0;
    virtual uint32_t GetSize() = 0;

};

#endif // IBUFFER_OBJECT_H
