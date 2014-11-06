#pragma once

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class Quad
{
private:
    float m_size;
public:
    BufferObject<glm::vec3> * pos;
    BufferObject<glm::vec2> * tex_coords;
    IndexBufferObject<uint32_t> * indices;

    std::shared_ptr<Mesh> glmesh;

    Quad(float size=1.0f)
    {
        this->m_size=size;
    }

    virtual ~Quad(){};

    virtual bool Init()
    {
        glmesh = share(new Mesh());
        pos = new BufferObject<glm::vec3>();
        pos->data.resize(4);
        pos->data[0]=glm::vec3(-1, 1, 0);
        pos->data[1]=glm::vec3(1, 1, 0);
        pos->data[2]=glm::vec3(-1,-1, 0);
        pos->data[3]=glm::vec3(1,-1, 0);

        tex_coords = new BufferObject<glm::vec2>();
        tex_coords->data.resize(4);
        tex_coords->data[0] = glm::vec2(0,1);
        tex_coords->data[1] = glm::vec2(1,1);
        tex_coords->data[2] = glm::vec2(0,0);
        tex_coords->data[3] = glm::vec2(1,0);

        indices = new IndexBufferObject<uint32_t>();
        indices->data.resize(6);
        indices->data[0]=0;
        indices->data[1]=2;
        indices->data[2]=1;

        indices->data[3]=2;
        indices->data[4]=3;
        indices->data[5]=1;

        glmesh->buffers.resize(3);
        glmesh->buffers[0] = pos;
        glmesh->buffers[1] = tex_coords;
        glmesh->buffers[2] = indices;


        glmesh->Init();
        return true;
    }

    void SetUV(vector<glm::vec2> uvs)
    {
        glBindBuffer(GL_ARRAY_BUFFER,glmesh->buffers[1]->Id);
        glBufferData(GL_ARRAY_BUFFER, 4*sizeof(uvs[0]), &uvs[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Render()
    {
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
};
