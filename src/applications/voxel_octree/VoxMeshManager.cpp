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
}

Map & VoxMeshManager::GetMeshes()
{
    return m_map;
}

void VoxMeshManager::ClearBuildNodes()
{
    loopxyz(32,32,32)
    m_buildNodes[x][y][z].size = 0;
}

void VoxMeshManager::SetBuildNode(const MNode & node)
{
    uint32_t x,y,z;
    decodeMK(node.start&LOCAL_VOXEL_MASK, x, y, z);
    m_buildNodes[x][y][z].start = node.start;
    m_buildNodes[x][y][z].size = 1;
}

bool vf_equals(const MNode & n1, const MNode & n2)
{
    return n1.size == 1 && n1.size == n2.size;
}


struct MaskNode
{
    uint8_t frontFace:1;
    uint8_t backFace:1;
    uint8_t align:6;

    operator bool()
    {
        return (frontFace || backFace);
    }
};


#define clear_mask(mask) loop(i,32) loop(j,32) {mask[i][j].frontFace = mask[i][j].backFace = false;}

uint32_t length(uint32_t x, uint32_t y, MaskNode mask[32][32], bool front = true)
{
    for(uint32_t i = x; i < 32; i++)
        if(front?mask[i][y].frontFace==false:mask[i][y].backFace==false)
            return i-x;

    return 32-x;
}

uint32_t height(uint32_t x, uint32_t y, uint32_t len, MaskNode mask[32][32], bool front = true)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < 32; i++)
        if(length(x,i,mask,front)==len)
            h++;
        else
            break;

    return h;
}

inline void VoxMeshManager::BuildSliceMask(uint32_t dim, uint32_t slice, MaskNode mask[32][32])
{
    MNode tmpNode;

    switch(dim)
    {
    case 0:
    {
        loop(y,32) loop(x,32)
        {
            MaskNode & n = mask[x][y];

            GetBuildNode(tmpNode,x,y,slice);

            if(tmpNode.size==1)
            {
                GetBuildNode(tmpNode,x,y,slice+1);
                n .frontFace = (tmpNode.size==0);

                GetBuildNode(tmpNode,x,y,slice-1);
                n .backFace = (tmpNode.size==0);
            }
        }
        break;
    }
    case 1:
    {
        loop(y,32) loop(x,32)
        {
            MaskNode & n = mask[x][y];

            GetBuildNode(tmpNode,x,slice,y);

            if(tmpNode.size==1)
            {
                GetBuildNode(tmpNode,x,slice+1,y);
                n.frontFace = (tmpNode.size==0);

                GetBuildNode(tmpNode,x,slice-1,y);
                n.backFace = (tmpNode.size==0);
            }
        }
        break;
    }
    case 2:
    {
        loop(y,32) loop(x,32)
        {
            MaskNode & n = mask[x][y];

            GetBuildNode(tmpNode,slice,x,y);
            if(tmpNode.size==1)
            {
                GetBuildNode(tmpNode,slice+1,x,y);
                n.frontFace = (tmpNode.size==0);

                GetBuildNode(tmpNode,slice-1,x,y);
                n.backFace = (tmpNode.size==0);
            }
        }
        break;
    }
    }
}

#define TOTAL_HEIGHT 32
#define TOTAL_WIDTH  32

struct ScanArea
{
    glm::ivec2 start, end;

    ScanArea(){}
    ScanArea(glm::ivec2 startArea, glm::ivec2 endArea)
    {
        start = startArea;
        end = endArea;
    }

    bool scanRight()
    {
        return end.x<TOTAL_WIDTH-1;
    }

    bool scanDown()
    {
        return end.x<TOTAL_WIDTH-1;
    }
};

void VoxMeshManager::GreedyBuildChunk(Mesh* mesh, const glm::vec3 & offset)
{
    MaskNode mask[32][32];
    MaskNode mn;
    glm::ivec2 qstart, qdims, qbstart, qbdims;

    loopr(dim, 0, 3)
    {
        clear_mask(mask);
        loop(z, 32)
        {
            BuildSliceMask(dim,z,mask);

            loop(y, 32)
            {
                loop(x, 32)
                {
                    mn = mask[x][y];

                    if(mn.frontFace)
                    {
                        qstart.x=x;
                        qstart.y=y;
                        qdims.x =length(x,y,mask);
                        qdims.y =height(x,y,qdims.x,mask);

                        AddFaceToMesh(mesh, true, dim, z, qstart, qdims, offset);
                        faceCount++;

                        for(uint32_t yi = qstart.y; yi < qstart.y+qdims.y; yi++)
                        for(uint32_t xi = qstart.x; xi < qstart.x+qdims.x; xi++)
                            mask[xi][yi].frontFace=false;
                    }
                    if(mn.backFace)
                    {
                        qbstart.x=x;
                        qbstart.y=y;
                        qbdims.x =length(x,y,mask,false);
                        qbdims.y =height(x,y,qbdims.x,mask,false);

                        AddFaceToMesh(mesh, false, dim, z, qbstart, qbdims, offset);
                        faceCount++;

                        for(uint32_t yi = qbstart.y; yi < qbstart.y+qbdims.y; yi++)
                        for(uint32_t xi = qbstart.x; xi < qbstart.x+qbdims.x; xi++)
                            mask[xi][yi].backFace=false;
                    }
                }
            }
        }
    }
}

void VoxMeshManager::AddFaceToMesh(Mesh* mesh, bool frontFace, uint32_t dir, uint32_t slice, glm::ivec2 start, glm::ivec2 dims, glm::vec3 offset)
{
    glm::vec3 face[4];

    switch(dir)
    {
    case 0: //xy
    {
        if(frontFace)
        {
            face[0]=glm::vec3(start.x,          start.y,              slice+1)+offset;
            face[1]=glm::vec3(start.x+dims.x,   start.y,              slice+1)+offset;
            face[2]=glm::vec3(start.x+dims.x,   start.y+dims.y,       slice+1)+offset;
            face[3]=glm::vec3(start.x,          start.y+dims.y,       slice+1)+offset;
        }
        else
        {
            face[3]=glm::vec3(start.x,          start.y,             slice)+offset;
            face[2]=glm::vec3(start.x+dims.x,   start.y,             slice)+offset;
            face[1]=glm::vec3(start.x+dims.x,   start.y+dims.y,      slice)+offset;
            face[0]=glm::vec3(start.x,          start.y+dims.y,      slice)+offset;
        }
        break;
    }
    case 1: //xz
    {
        if(frontFace)
        {
            face[3]=glm::vec3(start.x,         slice+1,                start.y)+offset;
            face[2]=glm::vec3(start.x+dims.x,  slice+1,                start.y)+offset;
            face[1]=glm::vec3(start.x+dims.x,  slice+1,                start.y+dims.y)+offset;
            face[0]=glm::vec3(start.x,         slice+1,                start.y+dims.y)+offset;
        }
        else
        {
            face[0]=glm::vec3(start.x,         slice,                  start.y)+offset;
            face[1]=glm::vec3(start.x+dims.x,  slice,                  start.y)+offset;
            face[2]=glm::vec3(start.x+dims.x,  slice,                  start.y+dims.y)+offset;
            face[3]=glm::vec3(start.x,         slice,                  start.y+dims.y)+offset;
        }

        break;
    }
    case 2: //yz
    {
        if(frontFace)
        {
            face[0]=glm::vec3(slice+1, start.x,         start.y      )+offset;
            face[1]=glm::vec3(slice+1, start.x +dims.x, start.y      )+offset;
            face[2]=glm::vec3(slice+1, start.x +dims.x, start.y+dims.y)+offset;
            face[3]=glm::vec3(slice+1, start.x,         start.y+dims.y)+offset;
        }
        else
        {
            face[3]=glm::vec3(slice, start.x,         start.y      )+offset;
            face[2]=glm::vec3(slice, start.x +dims.x, start.y      )+offset;
            face[1]=glm::vec3(slice, start.x +dims.x, start.y+dims.y)+offset;
            face[0]=glm::vec3(slice, start.x,         start.y+dims.y)+offset;
        }

        break;
    }
    default:
        break;
    }

    AddQuadToMesh(mesh,face);
}

void VoxMeshManager::AddQuadToMesh(Mesh* mesh, const glm::vec3 * face)
{
    BufferObject<glm::vec3> *vbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) mesh->buffers[Mesh::INDICES];
    BufferObject<glm::vec3> *cbo = (BufferObject<glm::vec3> *) mesh->buffers[Mesh::COLOR];

    uint32_t indicesStart = vbo->data.size();
    glm::vec3 color(0,150.0f/255.f,1.0f);

    vbo->data.push_back(face[0]);
    vbo->data.push_back(face[1]);
    vbo->data.push_back(face[2]);
    vbo->data.push_back(face[3]);

    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);
    cbo->data.push_back(color);

    ibo->data.push_back(indicesStart);
    ibo->data.push_back(indicesStart+2);
    ibo->data.push_back(indicesStart+3);

    ibo->data.push_back(indicesStart);
    ibo->data.push_back(indicesStart+1);
    ibo->data.push_back(indicesStart+2);
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

void VoxMeshManager::GetBuildNode(MNode & n, uint32_t x, uint32_t y, uint32_t z)
{
    if(x>31 || y>31 || z>31)
    {
        n.size = 0;
        n.start = 0;
    }
    else
    {
        n.size=m_buildNodes[x][y][z].size;
        n.start=m_buildNodes[x][y][z].start;
    }
}

void VoxMeshManager::GenAllChunks()
{
    if(m_octree->GetChildNodes().size()==0)
        return;

    faceCount = 0;
    ClearBuildNodes();


    uint32_t nodeChunk, x, y, z;
    uint32_t currentChunkMortonKey = nodeChunk = m_octree->GetChildNodes()[0].start & CHUNK_MASK;

    for( auto it = m_octree->GetChildNodes().begin(); it != m_octree->GetChildNodes().end(); it++ )
    {
        MNode & node = (*it);
        nodeChunk = node.start & CHUNK_MASK; /// get chunk (size 32x32x32)

        if(nodeChunk!=currentChunkMortonKey) /// THIS PIECE OF CODE IS GARBAGE, PLS FIX
        {
            decodeMK(currentChunkMortonKey,x,y,z);
            auto mesh = CreateEmptyMesh();
            m_map[currentChunkMortonKey] = mesh;
            GreedyBuildChunk(mesh.get(), glm::vec3(x,y,z));
            ClearBuildNodes();

            currentChunkMortonKey = nodeChunk;
        }

        SetBuildNode(node);
    }


    decodeMK(nodeChunk,x,y,z);
    auto mesh = CreateEmptyMesh();
    GreedyBuildChunk(mesh.get(), glm::vec3(x,y,z));
    m_map[nodeChunk] = mesh;

    for( MapIterator it = m_map.begin(); it != m_map.end(); it++ )
        it->second->UploadBuffers();

    std::cout << "Added " << faceCount << " faces to mesh" << std::endl;
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

    mesh->buffers[Mesh::POSITION] = new BufferObject<glm::vec3>();
    mesh->buffers[Mesh::INDICES] = new IndexBufferObject<uint32_t>();
    mesh->buffers[Mesh::COLOR] = new BufferObject<glm::vec3>();
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

    GreedyBuildChunk(mesh,glm::vec3(x,y,z));

    mesh->UploadBuffers();
}

void VoxMeshManager::RenderAllMeshes()
{
    for( MapIterator it = m_map.begin(); it != m_map.end(); it++ )
        it->second->Render();
}



