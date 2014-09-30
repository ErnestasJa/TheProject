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

void VoxMeshManager::BuildChunk(Mesh* mesh)
{
    uint8_t sides;

    loopxyz(32,32,32)
    if(m_buildNodes[x][y][z].size>0)
    {
        sides = GetVisibleBuildNodeSides(x,y,z);
        if(sides)
            AddVoxelToMesh( mesh, m_buildNodes[x][y][z], sides);
    }

    loopxyz(32,32,32) m_buildNodes[x][y][z].size=0;
}

bool vf_equals(const MNode & n1, const MNode & n2)
{
    return n1.size == 1 && n1.size == n2.size;
}


struct MaskNode
{
    uint8_t exists:1;
    uint8_t frontFace:1;
    uint8_t backFace:1;
    uint8_t align:5;

    MaskNode & operator = (bool value)
    {
        exists = value;
        return *this;
    }

    operator bool()
    {
        return exists==1 && (frontFace || backFace);
    }
};


#define clear_mask(mask) loop(i,32) loop(j,32) {mask[i][j].frontFace = false;mask[i][j].backFace = false; mask[i][j].exists = false;}

uint32_t length(uint32_t x, uint32_t y, MaskNode mask[32][32], bool front = true)
{
    for(uint32_t i = x; i < 32; i++)
        if(!mask[i][y].exists || (front?mask[i][y].frontFace==false:mask[i][y].backFace==false))
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

void VoxMeshManager::GreedyBuildChunk(Mesh* mesh, const glm::vec3 & offset)
{
    MaskNode mask[32][32];
    MaskNode mn;
    MNode tmpNode;
    glm::vec3 face[4];
    glm::ivec2 qstart, qdims, qbstart, qbdims;

    uint32_t faceCount = 0;

    loopr(dim, 0, 3)
    {
        clear_mask(mask);
        loop(z, 32)
        {
            switch(dim)
            {
            case 0: //xy slices
            {
                loop(y,32) loop(x,32)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpNode,x,y,z);
                    n = (tmpNode.size==1);

                    GetBuildNode(tmpNode,x,y,z+1);
                    n .frontFace = (tmpNode.size==0);

                    GetBuildNode(tmpNode,x,y,z-1);
                    n .backFace = (tmpNode.size==0);
                }
                break;
            }
            case 1: //xz slices
            {
                loop(y,32) loop(x,32)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpNode,x,z,y);
                    n = (tmpNode.size==1);

                    GetBuildNode(tmpNode,x,z+1,y);
                    n.frontFace = (tmpNode.size==0);

                    GetBuildNode(tmpNode,x,z-1,y);
                    n.backFace = (tmpNode.size==0);
                }
                break;
            }
            case 2: //yz slices
            {
                loop(y,32) loop(x,32)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpNode,z,y,x);
                    n = (tmpNode.size==1);

                    GetBuildNode(tmpNode,z+1,y,x);
                    n.frontFace = (tmpNode.size==0);

                    GetBuildNode(tmpNode,z-1,y,x);
                    n.backFace = (tmpNode.size==0);
                }
                break;
            }
            default:
                break;
            }

            qstart.x = 0;
            qstart.y = 0;

            loop(y, 32)
            {
                loop(x, 32)
                {
                    mn = mask[x][y];
                    if(mn)
                    {
                        /*
                        *    We found our victim, lets find out how fat he is.
                        */
                        if(mn.backFace)
                        {
                            qstart.x=x;
                            qstart.y=y;
                            qdims.x =length(x,y,mask,false);
                            qdims.y =height(x,y,qdims.x,mask,false);
                        }
                        if(mn.frontFace)
                        {
                            qbstart.x=x;
                            qbstart.y=y;
                            qbdims.x =length(x,y,mask);
                            qbdims.y =height(x,y,qbdims.x,mask);
                        }

                        /*
                        *    Now that we know how fat that little prick is, lets erase his identity.
                        */
                        if(mn.backFace)
                        for(uint32_t yi = qstart.y; yi < qstart.y+qdims.y; yi++)
                            for(uint32_t xi = qstart.x; xi < qstart.x+qdims.x; xi++)
                            {
                                    mask[xi][yi].backFace=false;
                                    mask[xi][yi].exists=mask[xi][yi].frontFace;
                            }

                        if(mn.frontFace)
                        for(uint32_t yi = qbstart.y; yi < qbstart.y+qbdims.y; yi++)
                        for(uint32_t xi = qbstart.x; xi < qbstart.x+qbdims.x; xi++)
                        {
                                mask[xi][yi].frontFace=false;
                                mask[xi][yi].exists=mask[xi][yi].backFace;
                        }

                        switch(dim)
                        {
                        case 0: //xy
                        {
                            if(mn.backFace)
                            {
                                face[3]=glm::vec3(qstart.x,             qstart.y,           z)+offset;
                                face[2]=glm::vec3(qstart.x+qdims.x,     qstart.y,           z)+offset;
                                face[1]=glm::vec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z)+offset;
                                face[0]=glm::vec3(qstart.x,             qstart.y+qdims.y,   z)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[0]=glm::vec3(qbstart.x,            qbstart.y,              z+1)+offset;
                                face[1]=glm::vec3(qbstart.x+qbdims.x,   qbstart.y,              z+1)+offset;
                                face[2]=glm::vec3(qbstart.x+qbdims.x,   qbstart.y+qbdims.y,     z+1)+offset;
                                face[3]=glm::vec3(qbstart.x,            qbstart.y+qbdims.y,     z+1)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }
                            break;
                        }
                        case 1: //xz
                        {
                            if(mn.backFace)
                            {
                                face[0]=glm::vec3(qstart.x,         z,                  qstart.y)+offset;
                                face[1]=glm::vec3(qstart.x+qdims.x, z,                  qstart.y)+offset;
                                face[2]=glm::vec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y)+offset;
                                face[3]=glm::vec3(qstart.x,         z,                  qstart.y+qdims.y)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=glm::vec3(qbstart.x,         z+1,                qbstart.y)+offset;
                                face[2]=glm::vec3(qbstart.x+qbdims.x, z+1,                qbstart.y)+offset;
                                face[1]=glm::vec3(qbstart.x+qbdims.x, z+1,                qbstart.y+qbdims.y)+offset;
                                face[0]=glm::vec3(qbstart.x,         z+1,                qbstart.y+qbdims.y)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }
                            break;
                        }
                        case 2: //yz
                        {
                            if(mn.backFace)
                            {
                                face[0]=glm::vec3(z,                qstart.y,           qstart.x)+offset;
                                face[1]=glm::vec3(z,                qstart.y,           qstart.x+qdims.x)+offset;
                                face[2]=glm::vec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x)+offset;
                                face[3]=glm::vec3(z,                qstart.y+qdims.y,   qstart.x)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=glm::vec3(z+1,              qbstart.y,           qbstart.x)+offset;
                                face[2]=glm::vec3(z+1,              qbstart.y,           qbstart.x+qbdims.x)+offset;
                                face[1]=glm::vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x+qbdims.x)+offset;
                                face[0]=glm::vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x)+offset;
                                AddQuadToMesh(mesh,face);
                                faceCount++;
                            }
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }

    //std::cout << "Added " << faceCount << " faces to mesh" << std::endl;
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


