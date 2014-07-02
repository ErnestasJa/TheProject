#pragma once

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class quad
{
private:
    float m_size;
public:
    buffer_object<glm::vec3> * pos;
    buffer_object<glm::vec2> * tex_coords;
    index_buffer_object<uint32_t> * indices;

    std::shared_ptr<mesh> glmesh;

    quad(float size=1.0f)
    {
        this->m_size=size;
    }

    virtual ~quad(){};

    virtual bool generate()
    {
        glmesh = share(new mesh());
        pos = new buffer_object<glm::vec3>();
        pos->data.resize(4);
        pos->data[0]=glm::vec3(-1,-1,0); /// LL
        pos->data[1]=glm::vec3(1,-1,0); /// LR
        pos->data[2]=glm::vec3(1,1,0); /// UR
        pos->data[3]=glm::vec3(-1,1,0); /// UL

        tex_coords = new buffer_object<glm::vec2>();
        tex_coords->data.resize(4);
        tex_coords->data[0] = glm::vec2(0,0);
        tex_coords->data[1] = glm::vec2(1,0);
        tex_coords->data[2] = glm::vec2(1,1);
        tex_coords->data[3] = glm::vec2(0,1);

        indices = new index_buffer_object<uint32_t>();
        indices->data.resize(6);
        indices->data[0]=0;
        indices->data[1]=3;
        indices->data[2]=1;

        indices->data[3]=1;
        indices->data[4]=3;
        indices->data[5]=2;

        glmesh->buffers.resize(3);
        glmesh->buffers[0] = pos;
        glmesh->buffers[1] = tex_coords;
        glmesh->buffers[2] = indices;


        init();
        return true;
    }

    bool init()
    {
        glmesh->init();
        return true;
    }

    void set_uv(std::vector<glm::vec2> uvs)
    {
        glBindBuffer(GL_ARRAY_BUFFER,glmesh->buffers[1]->id);
        glBufferData(GL_ARRAY_BUFFER, 4*sizeof(uvs[0]), &uvs[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw()
    {
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
};
