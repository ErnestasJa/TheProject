#include "Precomp.h"
#include "IBufferObject.h"

IBufferObject::IBufferObject(): Id(0), UsageHint(STATIC)
{

}

IBufferObject::~IBufferObject()
{
    if(Id!=0)
    {
        glDeleteBuffers(1,&Id);
    }
}
