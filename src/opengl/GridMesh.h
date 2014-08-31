#ifndef GRIDMESH_H_INCLUDED
#define GRIDMESH_H_INCLUDED

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class GridMesh:public Mesh
{
public:
    GridMesh(float density,int size=256)
    {
        BufferObject<glm::vec3> *pos=new BufferObject<glm::vec3>();
        BufferObject<glm::vec4> *col=new BufferObject<glm::vec4>();

        int halfsize=size/2;

        loop(i,size)
        {
            pos->data.push_back(glm::vec3(-halfsize+i*density,0,-size));
            pos->data.push_back(glm::vec3(-halfsize+i*density,0,size));
            col->data.push_back(glm::vec4(1,0,0,1));
            col->data.push_back(glm::vec4(1,0,0,1));


            pos->data.push_back(glm::vec3(-size,0,-halfsize+i*density));
            pos->data.push_back(glm::vec3(size,0,-halfsize+i*density));
            col->data.push_back(glm::vec4(0,0,1,1));
            col->data.push_back(glm::vec4(0,0,1,1));
        }

        this->buffers[Mesh::POSITION]=pos;
        this->buffers[Mesh::COLOR]=col;
        this->Init();
    }

    ~GridMesh() {}
private:
protected:
};

#endif // GRIDMESH_H_INCLUDED
