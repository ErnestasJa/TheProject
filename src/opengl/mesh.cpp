#include "precomp.h"
#include "mesh.h"
#include "ibuffer_object.h"

mesh::mesh(): vao(0)
{

}

void mesh::generate()
{
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i]->enabled)
        {
            std::cout << (buffers[i]->get_type()==ibuffer_object::DATA?"data buffer":"index buffer") << std::endl;
            buffers[i]->generate();

            if(buffers[i]->get_type()==ibuffer_object::DATA)
            {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i,buffers[i]->get_component_count(),buffers[i]->get_data_type(),GL_FALSE,0,0);
            }
        }
    }

    glBindVertexArray(0);
}

void mesh::disable_empty_buffers()
{
    for(uint32_t i = 0; i < buffers.size(); i++)
    if(buffers[i]->get_size()==0)
        buffers[i]->enabled = false;
}
