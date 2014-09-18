#include "precomp.h"
#include "VoxMeshManager.h"
#include "Morton.h"
#include "stdlib.h"

VoxMeshManager::VoxMeshManager(MortonOctTree<10> * octree, uint32_t level)
{
    m_octree = octree;
    m_level = level;
    ClearBuildNodes();
}

VoxMeshManager::~VoxMeshManager()
{
    //dtor
}

Map & VoxMeshManager::GetMeshes()
{
    return m_map;
}

void VoxMeshManager::ClearBuildNodes()
{
    loop(i,32)
    loop(j,32)
    loop(k,32)
        m_buildNodes[i][j][k].size = 0;
}

void VoxMeshManager::SetBuildNode(const MNode & node)
{
    uint32_t x,y,z;
    decodeMK(node.start&LOCAL_VOXEL_MASK, x, y, z);
    m_buildNodes[x][y][z].start = node.start;
    m_buildNodes[x][y][z].size = 1;
}

uint8_t VoxMeshManager::GetVisibleBuildNodeSides(uint32_t x, uint32_t y, uint32_t z)
{
    uint8_t sides = 0;

    if(x==0 || m_buildNodes[x-1][y][z].size == 0 )
        sides |= RIGHT;

    if(x==31 || m_buildNodes[x+1][y][z].size == 0 )
        sides |= LEFT;

    if(y==0 || m_buildNodes[x][y-1][z].size == 0 )
        sides |= BOTTOM;

    if(y==31 || m_buildNodes[x][y+1][z].size == 0 )
        sides |= TOP;

    if(z==0 || m_buildNodes[x][y][z-1].size == 0 )
        sides |= BACK;

    if(z==31 || m_buildNodes[x][y][z+1].size == 0 )
        sides |= FRONT;

    return sides;
}

void VoxMeshManager::GenAllChunks()
{
    uint32_t currentChunkMortonKey = ~0;
    int8_t sides;
    MeshPtr mesh;

    for( auto it = m_octree->GetChildNodes().begin(); it != m_octree->GetChildNodes().end(); it++ )
    {
        MNode & node = (*it);

        const uint32_t nodeChunk = node.start & CHUNK_MASK; /// get chunk (size 32x32x32)

        if(nodeChunk!=currentChunkMortonKey)
        {
            mesh = CreateEmptyMesh();
            m_map[nodeChunk] = mesh;
            currentChunkMortonKey = nodeChunk;

            loop(i,32)
                loop(j,32)
                    loop(k,32)
                        if(m_buildNodes[k][j][i].size>0)
                        {
                            sides = GetVisibleBuildNodeSides(k,j,i);
                            if(sides)
                                AddVoxelToMesh( mesh.get(), m_buildNodes[k][j][i], sides);
                        }
            ClearBuildNodes();
        }

        SetBuildNode(node);
    }



    for( MapIterator it = m_map.begin(); it != m_map.end(); it++ )
        it->second->UploadBuffers();
}

void VoxMeshManager::AddVoxelToMesh(Mesh* mesh, vector<MNode>::iterator nodeIt)
{
    MNode & node = (*nodeIt);

    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) mesh->buffers[Mesh::INDICES];
    BufferObject<glm::vec3> *vbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::POSITION];
    BufferObject<glm::vec3> *cbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::COLOR];

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


void VoxMeshManager::AddVoxelToMesh(Mesh* mesh, const MNode & node, uint8_t sides)
{
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) mesh->buffers[Mesh::INDICES];
    BufferObject<glm::vec3> *vbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::POSITION];
    BufferObject<glm::vec3> *cbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::COLOR];

    uint32_t x, y, z;
    decodeMK(node.start, x,y,z);

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

MeshPtr VoxMeshManager::CreateEmptyMesh()
{
    MeshPtr mesh = MeshPtr(new Mesh());

    IndexBufferObject<uint32_t> * ibo = new IndexBufferObject<uint32_t>();
    BufferObject<glm::vec3> *vbo = new BufferObject<glm::vec3>();
    BufferObject<glm::vec3> *cbo = new BufferObject<glm::vec3>();

    mesh->buffers[Mesh::POSITION] = vbo;
    mesh->buffers[Mesh::INDICES] = ibo;
    mesh->buffers[Mesh::COLOR] = cbo;
    mesh->Init();

    return mesh;
}

void VoxMeshManager::ClearMesh(Mesh * mesh)
{
    auto ibo = (IndexBufferObject<uint32_t> *)mesh->buffers[Mesh::INDICES];
    auto vbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::POSITION];
    auto cbo = (BufferObject<glm::vec3> *)mesh->buffers[Mesh::COLOR];

    ibo->data.clear();
    vbo->data.clear();
    cbo->data.clear();
}

void VoxMeshManager::RebuildChunk(uint32_t chunk)
{
    uint32_t x,y,z;
    decodeMK(chunk,x,y,z);
    printf("Rebuilding chunk [%u,%u,%u]\n", x, y, z);

    auto it = boost::range::lower_bound(m_octree->GetChildNodes(), MNode(chunk));
    const uint32_t fchunk = (it->start&CHUNK_MASK);

    MapIterator mit;

    if(fchunk==chunk) /// chunk exists
        mit = m_map.find(chunk);
    else
    {
        m_map.erase(fchunk);
        return;
    }

    Mesh * mesh = nullptr;

    if(mit == m_map.end())
    {
        MeshPtr m = CreateEmptyMesh();
        m_map[chunk]=m;
        mesh = m.get();
    }
    else
    {
        mesh = mit->second.get();
        ClearMesh(mesh);
    }

    ClearBuildNodes();
    for( ; it != m_octree->GetChildNodes().end(); it++ )
    {
        MNode & node = (*it);
        const uint32_t nodeChunk = node.start & CHUNK_MASK; /// get chunk (size 32x32x32)

        if(nodeChunk!=fchunk)
            break;

        SetBuildNode( node );
    }

    loop(i,32)
        loop(j,32)
            loop(k,32)
                if(m_buildNodes[k][j][i].size>0)
                {
                    uint8_t sides = GetVisibleBuildNodeSides(k,j,i);
                    if(sides)
                        AddVoxelToMesh( mesh, m_buildNodes[k][j][i], sides);
                }

    mesh->UploadBuffers();
}

void VoxMeshManager::RenderAllMeshes()
{
    for( MapIterator it = m_map.begin(); it != m_map.end(); it++ )
        it->second->Render();
}
