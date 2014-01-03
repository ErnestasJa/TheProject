#ifndef ISG_OBJECT_H
#define ISG_OBJECT_H

#include "sg_object_types.h"

namespace sg
{

class isg_object
{
protected:
    glm::mat4x4 m_transform;
    std::string m_name;

public:
    isg_object();
    virtual ~isg_object();

    virtual glm::mat4x4 & get_transform();
    virtual std::string & get_name();

public:
    virtual uint32_t get_type() = 0;
    virtual void render()=0;
};

typedef std::shared_ptr<isg_object> sg_object_ptr;

}

#endif // ISG_OBJECT_H
