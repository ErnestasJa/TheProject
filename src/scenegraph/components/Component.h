#ifndef COMPONENT_H
#define COMPONENT_H
#include "ComponentTypes.h"
#include "scenegraph/ISGObject.h"

namespace sg
{
class Component
{
public:
    Component(isg_object * obj);
    virtual ~Component();

    virtual void OnUpdate(float deltaTime) = 0;
    virtual uint32_t GetType() = 0;
    isg_object * GetObject();

private:
    isg_object * p_object;
};
}

#endif // COMPONENT_H
