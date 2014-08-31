#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class CubeMesh
{
private:
    float m_size;
public:
    BufferObject<glm::vec3> * pos;
    BufferObject<glm::vec4> * col;
    BufferObject<glm::vec2> * tex_coords;
    IndexBufferObject<uint32_t> * indices;
    glm::vec4 m_color;

    std::shared_ptr<Mesh> glmesh;

    CubeMesh(float size=1.0f,glm::vec4 color=glm::vec4(1))
    {
        this->m_size=size;
        this->m_color=color;
        Init();
    }

    ~CubeMesh() {};

    bool Init()
    {
        glmesh = share(new Mesh());

        pos = new BufferObject<glm::vec3>();

        float halfsize=m_size/2.f;

        /// - - +
        glm::vec3 p1(-halfsize, -halfsize, halfsize);
        /// + - +
        glm::vec3 p2(halfsize, -halfsize, halfsize);
        /// + + +
        glm::vec3 p3(halfsize, halfsize, halfsize);
        /// - + +
        glm::vec3 p4(-halfsize, halfsize, halfsize);

        /// - - -
        glm::vec3 p5(-halfsize, -halfsize, -halfsize);
        /// + - -
        glm::vec3 p6(halfsize, -halfsize, -halfsize);
        /// + + -
        glm::vec3 p7(halfsize, halfsize, -halfsize);
        /// - + -
        glm::vec3 p8(-halfsize, halfsize, -halfsize);

        pos->data.resize(8);
        pos->data[0]=p1;
        pos->data[1]=p2;
        pos->data[2]=p3;
        pos->data[3]=p4;
        pos->data[4]=p5;
        pos->data[5]=p6;
        pos->data[6]=p7;
        pos->data[7]=p8;

        col = new BufferObject<glm::vec4>();

        col->data.resize(8);
        col->data[0]=m_color;
        col->data[1]=m_color;
        col->data[2]=m_color;
        col->data[3]=m_color;
        col->data[4]=m_color;
        col->data[5]=m_color;
        col->data[6]=m_color;
        col->data[7]=m_color;

        indices = new IndexBufferObject<uint32_t>();

        indices->data.resize(36);
        // Front
        indices->data[0]=0;
        indices->data[1]=1;
        indices->data[2]=2;
        indices->data[3]=2;
        indices->data[4]=3;
        indices->data[5]=0;
        // Top
        indices->data[6]=3;
        indices->data[7]=2;
        indices->data[8]=6;
        indices->data[9]=6;
        indices->data[10]=7;
        indices->data[11]=3;
        // Back
        indices->data[12]=7;
        indices->data[13]=6;
        indices->data[14]=5;
        indices->data[15]=5;
        indices->data[16]=4;
        indices->data[17]=7;
        // Bottom
        indices->data[18]=4;
        indices->data[19]=5;
        indices->data[20]=1;
        indices->data[21]=1;
        indices->data[22]=0;
        indices->data[23]=4;
        // Left
        indices->data[24]=4;
        indices->data[25]=0;
        indices->data[26]=3;
        indices->data[27]=3;
        indices->data[28]=7;
        indices->data[29]=4;
        // Right
        indices->data[30]=1;
        indices->data[31]=5;
        indices->data[32]=6;
        indices->data[33]=6;
        indices->data[34]=2;
        indices->data[35]=1;

        glmesh->buffers[Mesh::POSITION] = pos;
        glmesh->buffers[Mesh::COLOR] = col;
        glmesh->buffers[Mesh::INDICES] = indices;

        glmesh->Init();
        return true;
    }

    void Render(bool wireframe)
    {
        if(wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        glmesh->Render();
        if(wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
};

#endif // CUBEMESH_H
