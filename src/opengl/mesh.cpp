#include "Precomp.h"
#include "Mesh.h"
#include "IBufferObject.h"
#include "BufferObject.h"
#include "utility/Util.h"

void animation::set_frame(uint32_t frame)
{
    const uint32_t offset = (frame%this->frames.size());

    for(uint32_t i = 0; i < this->bones.size(); i++)
    {
        if(this->bones[i].parent >= 0) current_frame[i] = mul(current_frame[this->bones[i].parent],frames[offset][i]);
        else current_frame[i] = frames[offset][i];
    }
}

void animation::set_interp_frame(float f)
{
    if(this->frames.size() <= 0) return;

    int frame1 = (int)floor(f),
        frame2 = frame1 + 1;

    float frameoffset = f - frame1;
    frame1 %= this->frames.size();
    frame2 %= this->frames.size();
    frame &mat1 = frames[frame1],
              &mat2 = frames[frame2];
    // Interpolate matrixes between the two closest frames and concatenate with parent matrix if necessary.
    // Concatenate the result with the inverse of the base pose.
    // You would normally do animation blending and inter-frame blending here in a 3D engine.

    for(uint32_t i = 0; i < bones.size(); i++)
    {
        glm::mat3x4 mat = mat1[i]*(1 - frameoffset) + mat2[i]*frameoffset;
        if(bones[i].parent >= 0) current_frame[i] = mul(current_frame[bones[i].parent],mat);
        else current_frame[i] = mat;
    }
}

mesh::mesh(): vao(0), anim(nullptr)
{
    buffers.resize(BUFFER_COUNT);

    for(uint32_t i=0; i<BUFFER_COUNT; i++)
        buffers[i]=nullptr;
}
mesh::~mesh()
{
    for(ibuffer_object * b : buffers)
        if(b)
            delete b;
}

void mesh::init()
{
    disable_empty_buffers();

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i])
        {
            buffers[i]->init();


            if(buffers[i]->get_size())
                buffers[i]->upload();

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

}

void mesh::Render(uint32_t sub_mesh_index)
{
    glBindVertexArray(vao);

    if(sub_meshes.size()!=0)
        glDrawElements(GL_TRIANGLES,sub_meshes[sub_mesh_index].num_indices,GL_UNSIGNED_INT,(void*)(sizeof(uint32_t)*sub_meshes[sub_mesh_index].start));
    else
        glDrawElements(GL_TRIANGLES,static_cast<index_buffer_object<uint32_t>*>(buffers[INDICES])->data.size(),GL_UNSIGNED_INT,(void*)(0));

    glBindVertexArray(0);
}

void mesh::Render()
{
    glBindVertexArray(vao);

    if(sub_meshes.size()!=0)
    {
        for(uint32_t i = 0; i < sub_meshes.size(); i++)
        {
            glDrawElements(GL_TRIANGLES,sub_meshes[i].num_indices,GL_UNSIGNED_INT,(void*)(sizeof(uint32_t)*sub_meshes[i].start));
        }
    }
    else
    {
        glDrawElements(GL_TRIANGLES,buffers[INDICES]->get_size(),GL_UNSIGNED_INT,0);
    }

    glBindVertexArray(0);
}

void mesh::render_lines()
{
    glBindVertexArray(vao);

    uint32_t size = buffers[POSITION]->get_size();

    if(buffers[POSITION]&&buffers[POSITION]->get_size())
        glDrawArrays(GL_LINES,0,size);

    glBindVertexArray(0);
}

void mesh::render_triangle_strip()
{
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES,0,buffers[POSITION]->get_size());

    glBindVertexArray(0);
}

void mesh::upload_buffers()
{
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i])
        {
            ///enable on upload. MIGHT NOT BE A REALLY GOOD SOLUTION
            if(buffers[i]->get_size()>0)
            {

                buffers[i]->upload();

                if(buffers[i]->get_type()==ibuffer_object::DATA)
                {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(i,buffers[i]->get_component_count(),buffers[i]->get_data_type(),GL_FALSE,0,0);
                }
            }
        }
    }
    glBindVertexArray(0);
}
