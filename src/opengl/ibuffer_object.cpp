#include "precomp.h"
#include "ibuffer_object.h"

ibuffer_object::ibuffer_object(): id(0), enabled(true)
{

}

ibuffer_object::~ibuffer_object()
{
    if(id!=0)
    {
        glDeleteBuffers(1,&id);
    }
}
