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

Mesh::Mesh(): vao(0), anim(nullptr)
{
    buffers.resize(BUFFER_COUNT);

    for(uint32_t i=0; i<BUFFER_COUNT; i++)
        buffers[i]=nullptr;
}
Mesh::~Mesh()
{
    glBindVertexArray(vao);
    for(IBufferObject * b : buffers)
        delete b;
    glDeleteVertexArrays(1,&vao);
}

void Mesh::Init()
{
    DisableEmptyBuffers();

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i])
        {
            buffers[i]->Init();

            if(buffers[i]->GetSize())
                buffers[i]->Upload();

            if(buffers[i]->GetType()==IBufferObject::DATA)
            {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i,buffers[i]->GetComponentCount(),buffers[i]->GetDataType(),GL_FALSE,0,0);
            }
        }
    }

    glBindVertexArray(0);
}

void Mesh::DisableEmptyBuffers()
{

}

void Mesh::Render(uint32_t sub_mesh_index)
{
    glBindVertexArray(vao);

    if(sub_meshes.size()!=0)
        glDrawElements(GL_TRIANGLES,sub_meshes[sub_mesh_index].num_indices,GL_UNSIGNED_INT,(void*)(sizeof(uint32_t)*sub_meshes[sub_mesh_index].start));
    else
        glDrawElements(GL_TRIANGLES,static_cast<IndexBufferObject<uint32_t>*>(buffers[INDICES])->data.size(),GL_UNSIGNED_INT,(void*)(0));

    glBindVertexArray(0);
}

void Mesh::Render()
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[INDICES]->Id);

    if(sub_meshes.size()!=0)
        for(uint32_t i = 0; i < sub_meshes.size(); i++)
            glDrawElements(GL_TRIANGLES,sub_meshes[i].num_indices,GL_UNSIGNED_INT,(void*)(sizeof(uint32_t)*sub_meshes[i].start));
    else
        glDrawElements(GL_TRIANGLES,buffers[INDICES]->GetSize(),GL_UNSIGNED_INT,0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::render_lines()
{
    glBindVertexArray(vao);

    uint32_t size = buffers[POSITION]->GetSize();

    if(buffers[POSITION]&&buffers[POSITION]->GetSize())
        glDrawArrays(GL_LINES,0,size);

    glBindVertexArray(0);
}

void Mesh::render_triangle_strip()
{
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES,0,buffers[POSITION]->GetSize());

    glBindVertexArray(0);
}

void Mesh::UploadBuffers()
{
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i])
        {
            ///enable on upload. MIGHT NOT BE A REALLY GOOD SOLUTION
            if(buffers[i]->GetSize()>0)
            {
                buffers[i]->Upload();

                if(buffers[i]->GetType()==IBufferObject::DATA)
                {
                    glEnableVertexAttribArray(i);
                    glVertexAttribPointer(i,buffers[i]->GetComponentCount(),buffers[i]->GetDataType(),GL_FALSE,0,0);
                }
            }
        }
    }
    glBindVertexArray(0);
}
