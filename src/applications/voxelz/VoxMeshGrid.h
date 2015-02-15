#ifndef VOXMESHGRID_H_INCLUDED
#define VOXMESHGRID_H_INCLUDED

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"

class VoxMeshGrid:public Mesh
{
public:
    VoxMeshGrid(const glm::vec3 &size,uint32_t density=8)
    {
        pos=new BufferObject<glm::vec3>();
        col=new BufferObject<glm::vec4>();

        glm::vec3 halfsize(size.x/2.f,size.y/2.f,size.z/2.f);

        for(float x=0.f; x<=size.x;)
        {
            for(float z=0.f; z<=size.z;)
            {
                for(float y=0.f; y<=size.y;)
                {
                    AddLine(glm::vec3(-halfsize.x+x,halfsize.y-y,-halfsize.z),glm::vec3(-halfsize.x+x,halfsize.y-y,halfsize.z));

                    AddLine(glm::vec3(-halfsize.x,halfsize.y-y,-halfsize.z+z),glm::vec3(halfsize.x,halfsize.y-y,-halfsize.z+z));

                    y+=1.f/density*size.y;
                }

                AddLine(glm::vec3(-halfsize.x+x,-halfsize.y,-halfsize.z+z),glm::vec3(-halfsize.x+x,halfsize.y,-halfsize.z+z));
                z+=1.f/density*size.z;
            }
            x+=1.f/density*size.x;
        }

        this->buffers[Mesh::POSITION]=pos;
        this->buffers[Mesh::COLOR]=col;
        this->Init();
    }

    virtual ~VoxMeshGrid() {}

    void AddLine(const glm::vec3& start,const glm::vec3& end,const glm::vec4 &color=glm::vec4(1.f))
    {
        pos->data.push_back(start);
        pos->data.push_back(end);
        col->data.push_back(color);
        col->data.push_back(color);
    }
private:
    BufferObject<glm::vec3> *pos;
    BufferObject<glm::vec4> *col;
protected:
};

#endif // VOXMESHGRID_H_INCLUDED
