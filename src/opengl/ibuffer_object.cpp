#include "Precomp.h"
#include "IBufferObject.h"

IBufferObject::IBufferObject(): Id(0), usage_hint(STATIC)
{

}

IBufferObject::~IBufferObject()
{
    if(Id!=0)
    {
        glDeleteBuffers(1,&Id);
    }
}
