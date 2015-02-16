#ifndef GRIDMESH_H_INCLUDED
#define GRIDMESH_H_INCLUDED

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class GridMesh:public Mesh
{
public:
    GridMesh(float density,uint32_t size=256,uint32_t gridlines=16)
    {
        BufferObject<glm::vec3> *pos=new BufferObject<glm::vec3>();
        BufferObject<glm::vec4> *col=new BufferObject<glm::vec4>();

        int32_t halfsize=size/2;

        /// XAxis
        pos->data.push_back(glm::vec3(-halfsize,0,0));
        pos->data.push_back(glm::vec3(0,0,0));
        col->data.push_back(glm::vec4(1,0,0,1));
        col->data.push_back(glm::vec4(1,0,0,1));

        pos->data.push_back(glm::vec3(0,0,0));
        pos->data.push_back(glm::vec3(halfsize,0,0));
        col->data.push_back(glm::vec4(1,0.5,0.5,1));
        col->data.push_back(glm::vec4(1,0.5,0.5,1));

        /// YAxis
        pos->data.push_back(glm::vec3(0,-halfsize,0));
        pos->data.push_back(glm::vec3(0,0,0));
        col->data.push_back(glm::vec4(0,1,0,1));
        col->data.push_back(glm::vec4(0,1,0,1));

        pos->data.push_back(glm::vec3(0,0,0));
        pos->data.push_back(glm::vec3(0,halfsize,0));
        col->data.push_back(glm::vec4(0.5,1,0.5,1));
        col->data.push_back(glm::vec4(0.5,1,0.5,1));

        /// ZAxis
        pos->data.push_back(glm::vec3(0,0,-halfsize));
        pos->data.push_back(glm::vec3(0,0,0));
        col->data.push_back(glm::vec4(0,0,1,1));
        col->data.push_back(glm::vec4(0,0,1,1));

        pos->data.push_back(glm::vec3(0,0,0));
        pos->data.push_back(glm::vec3(0,0,halfsize));
        col->data.push_back(glm::vec4(0.5,0.5,1,1));
        col->data.push_back(glm::vec4(0.5,0.5,1,1));

        for(uint32_t i=0; i<=size; i++)
        {
            if(i!=halfsize)
            {

                pos->data.push_back(glm::vec3(-halfsize+i*density,0,-halfsize));
                pos->data.push_back(glm::vec3(-halfsize+i*density,0,halfsize));

                if(i%gridlines!=0)
                {
                    col->data.push_back(glm::vec4(0.5,0.5,0.5,1));
                    col->data.push_back(glm::vec4(0.5,0.5,0.5,1));
                }
                else
                {
                    col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
                    col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
                }

                pos->data.push_back(glm::vec3(-halfsize,0,-halfsize+i*density));
                pos->data.push_back(glm::vec3(halfsize,0,-halfsize+i*density));
                if(i%gridlines!=0)
                {
                    col->data.push_back(glm::vec4(0.5,0.5,0.5,1));
                    col->data.push_back(glm::vec4(0.5,0.5,0.5,1));
                }
                else
                {
                    col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
                    col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
                }
            }
            else if(i==size)
            {
                pos->data.push_back(glm::vec3(-halfsize+i*density,0,-halfsize));
                pos->data.push_back(glm::vec3(-halfsize+i*density,0,halfsize));
                col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
                col->data.push_back(glm::vec4(0.75,0.75,0.75,1));
            }
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

