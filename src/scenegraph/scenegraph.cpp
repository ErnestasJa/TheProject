#include "precomp.h"
#include "scenegraph.h"

namespace sg
{

scenegraph::scenegraph()
{
    //ctor
}

scenegraph::~scenegraph()
{
    //dtor
}

void scenegraph::add_object(sg_object_ptr obj)
{
    m_objects.push_back(obj);
}

void scenegraph::pre_render()
{

}

void scenegraph::render_all()
{
    pre_render();

    for(uint32_t i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->render();
    }

    post_render();
}

void scenegraph::post_render()
{

}

}
