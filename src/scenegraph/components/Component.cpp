#include "precomp.h"
#include "Component.h"

namespace sg
{
Component::Component(isg_object * obj)
{
    p_object = obj;
}

Component::~Component()
{

}
}
