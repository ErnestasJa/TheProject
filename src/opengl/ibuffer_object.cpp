#include "Precomp.h"
#include "IBufferObject.h"

ibuffer_object::ibuffer_object(): Id(0), usage_hint(STATIC)
{

}

ibuffer_object::~ibuffer_object()
{
    if(Id!=0)
    {
        glDeleteBuffers(1,&Id);
    }
}
