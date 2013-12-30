#pragma once

#include "opengl/mesh.h"
#include "opengl/shader.h"
#include "opengl/buffer_object.h"

class quad
{
public:
    buffer_object<glm::vec3> * pos;
    buffer_object<glm::vec2> * tex_coords;
    index_buffer_object<uint32_t> * indices;

    std::shared_ptr<mesh> glmesh;

    quad()
    {
        glmesh = share(new mesh());
        pos = new buffer_object<glm::vec3>();
        pos->data.resize(4);
        pos->data[0]=glm::vec3(-1,-1,0);
        pos->data[1]=glm::vec3(1,-1,0);
        pos->data[2]=glm::vec3(1,1,0);
        pos->data[3]=glm::vec3(-1,1,0);

        tex_coords = new buffer_object<glm::vec2>();
        tex_coords->data.resize(4);
        tex_coords->data[0] = glm::vec2(0,0);
        tex_coords->data[1] = glm::vec2(1,0);
        tex_coords->data[2] = glm::vec2(1,1);
        tex_coords->data[3] = glm::vec2(0,1);

        indices = new index_buffer_object<uint32_t>();
        indices->data.resize(6);
        indices->data[0]=0;
        indices->data[1]=1;
        indices->data[2]=3;

        indices->data[3]=3;
        indices->data[4]=1;
        indices->data[5]=2;

        glmesh->buffers.resize(3);
        glmesh->buffers[0] = pos;
        glmesh->buffers[1] = tex_coords;
        glmesh->buffers[2] = indices;
    }

    bool generate()
    {
        glmesh->generate();
        return true;
    }

    void draw()
    {
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
};
