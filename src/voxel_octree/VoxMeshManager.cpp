#include "precomp.h"
#include "VoxMeshManager.h"
#include "Morton.h"
#include "stdlib.h"

VoxMeshManager::VoxMeshManager(MortonOctTree<10> * octree, uint32_t level)
{
    m_octree = octree;
    m_level = level;
}

VoxMeshManager::~VoxMeshManager()
{
    //dtor
}

Map & VoxMeshManager::GetMeshes()
{
    return m_map;
}

void VoxMeshManager::GenAllChunks()
{

}

void VoxMeshManager::GenMesh(MeshPtr mesh)
{
    srand(12345);
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *)mesh->buffers[Mesh::INDICES];
    BufferObject<glm::vec3> *vbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::POSITION];
    BufferObject<glm::vec3> *cbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::COLOR];

    ibo->data.clear();
    vbo->data.clear();
    cbo->data.clear();

    for(auto it = m_octree->GetChildNodes().begin(); it != m_octree->GetChildNodes().end(); it++ )
    {
        AddVoxelToMesh(mesh.get(),it);
    }

    mesh->UploadBuffers();
}

void VoxMeshManager::AddVoxelToMesh(Mesh* mesh, vector<MNode>::iterator nodeIt)
{
    MNode & node = (*nodeIt);

    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *)mesh->buffers[Mesh::INDICES];
    BufferObject<glm::vec3> *vbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::POSITION];
    BufferObject<glm::vec3> *cbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::COLOR];

    uint32_t x, y, z;
    decodeMK(node.start, x,y,z);

    uint8_t sides = m_octree->GetVisibleSides(x,y,z,nodeIt);

    uint32_t si = vbo->data.size();

    //vbo->data.reserve(vbo->data.size()+8);
    vbo->data.push_back(glm::vec3(x,  y,  z));
    vbo->data.push_back(glm::vec3(x+1,y,  z));
    vbo->data.push_back(glm::vec3(x+1,y,  z+1));
    vbo->data.push_back(glm::vec3(x,  y,  z+1));

    vbo->data.push_back(glm::vec3(x,  y+1,z));
    vbo->data.push_back(glm::vec3(x+1,y+1,z));
    vbo->data.push_back(glm::vec3(x+1,y+1,z+1));
    vbo->data.push_back(glm::vec3(x,  y+1,z+1));

    ///color data
    glm::vec3 color((rand()%256)/255.0f,(rand()%256),(rand()%256)/255.0f);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);

    ///faces
    //ibo->data.reserve(ibo->data.size()+36);

    //back face
    if(CheckBit(sides,BACK))
    {
        ibo->data.push_back(si);
        ibo->data.push_back(si+4);
        ibo->data.push_back(si+1);

        ibo->data.push_back(si+1);
        ibo->data.push_back(si+4);
        ibo->data.push_back(si+5);
    }

    //left face
    if(CheckBit(sides,LEFT))
    {
        ibo->data.push_back(si+1);
        ibo->data.push_back(si+5);
        ibo->data.push_back(si+2);

        ibo->data.push_back(si+2);
        ibo->data.push_back(si+5);
        ibo->data.push_back(si+6);
    }

    //front face
    if(CheckBit(sides,FRONT))
    {
        ibo->data.push_back(si+2);
        ibo->data.push_back(si+6);
        ibo->data.push_back(si+3);

        ibo->data.push_back(si+3);
        ibo->data.push_back(si+6);
        ibo->data.push_back(si+7);
    }

    //right face
    if(CheckBit(sides,RIGHT))
    {
        ibo->data.push_back(si+3);
        ibo->data.push_back(si+7);
        ibo->data.push_back(si+0);

        ibo->data.push_back(si+0);
        ibo->data.push_back(si+7);
        ibo->data.push_back(si+4);
    }

    //bot face
    if(CheckBit(sides,BOTTOM))
    {
        ibo->data.push_back(si+0);
        ibo->data.push_back(si+2);
        ibo->data.push_back(si+3);

        ibo->data.push_back(si+0);
        ibo->data.push_back(si+1);
        ibo->data.push_back(si+2);
    }

    //top face
    if(CheckBit(sides,TOP))
    {
        ibo->data.push_back(si+6);
        ibo->data.push_back(si+4);
        ibo->data.push_back(si+7);

        ibo->data.push_back(si+5);
        ibo->data.push_back(si+4);
        ibo->data.push_back(si+6);
    }
}
