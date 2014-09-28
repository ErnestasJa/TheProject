#include "precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "Chunk.h"
#include "VoxelMesh.h"

VoxelMesh::VoxelMesh(uint8_t xsize,uint8_t ysize,uint8_t zsize):m_vox(boost::extents[xsize][ysize][zsize])
{
    m_xsize=xsize;
    m_ysize=ysize;
    m_zsize=zsize;
    m_dirty=false;
    m_indexTrack=0;
    m_vertexTrack=0;

    m_posBuf=new BufferObject<u8vec3>();
    m_colBuf=new BufferObject<u8vec4>();
    m_indBuf=new IndexBufferObject<uint32_t>();

    m_mesh = share(new Mesh());

    m_mesh->buffers[Mesh::POSITION] = m_posBuf;
    m_mesh->buffers[Mesh::COLOR] = m_colBuf;
    m_mesh->buffers[Mesh::INDICES] = m_indBuf;

    m_mesh->Init();
}

VoxelMesh::~VoxelMesh()
{
    delete m_colBuf;
    delete m_posBuf;
    delete m_indBuf;
}

bool VoxelMesh::Empty()
{
    return m_vox.size()==0;
}

void VoxelMesh::Render()
{
    if(m_dirty)
        Rebuild();

    if(!Empty()&&!m_dirty)
        m_mesh->Render();

}

void VoxelMesh::Cleanup()
{
    if(!Empty())
    {
        loop(x,m_xsize)loop(y,m_ysize)loop(z,m_zsize)m_vox[x][y][z].active=false;
        m_posBuf->data.clear();
        m_colBuf->data.clear();
        m_indBuf->data.clear();
        m_indexTrack=0;
        m_vertexTrack=0;
    }
}

void VoxelMesh::UpdateMesh()
{
    if(!Empty())
    {
        GreedyBuild(u8vec3(0));

        m_mesh->buffers[Mesh::POSITION] = m_posBuf;
        m_mesh->buffers[Mesh::COLOR] = m_colBuf;
        m_mesh->buffers[Mesh::INDICES] = m_indBuf;

        m_mesh->UploadBuffers();
    }
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

void VoxelMesh::GetBuildNode(Voxel &vox,uint8_t x,uint8_t y, uint8_t z)
{
    if(x>m_xsize-1 || y>m_ysize-1 || z>m_zsize-1)
    {
        vox.active=0;
        vox.type=0;
    }
    else
    {
        vox.active=m_vox[x][y][z].active;
        vox.type=m_vox[x][y][z].type;
    }
}

#define clear_mask(mask) loop(i,16) loop(j,16) {mask[i][j].frontFace = false;mask[i][j].backFace = false; mask[i][j].exists = false;}

uint32_t length(uint32_t x, uint32_t y, MaskNode mask[16][16], bool front = true)
{
    for(uint32_t i = x; i < 16; i++)
        if(!mask[i][y].exists || (front?mask[i][y].frontFace==false:mask[i][y].backFace==false))
            return i-x;

    return 16-x;
}

uint32_t height(uint32_t x, uint32_t y, uint32_t len, MaskNode mask[16][16], bool front = true)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < 16; i++)
        if(length(x,i,mask,front)==len)
            h++;
        else
            break;

    return h;
}

void VoxelMesh::GreedyBuild(const u8vec3 & offset)
{
    MaskNode mask[16][16];
    MaskNode mn;
    Voxel tmpVoxel;

    u8vec3 face[4];

    glm::ivec2 qstart, qdims, qbstart, qbdims;

    uint32_t faceCount = 0;

    for(uint32_t dim=0; dim<3; dim++)
    {
        clear_mask(mask);
        loop(z, 16)
        {
            switch(dim)
            {
            case 0: //xy slices
            {
                loop(y,16) loop(x,16)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpVoxel,x,y,z);
                    n = (tmpVoxel.active==1);

                    GetBuildNode(tmpVoxel,x,y,z+1);
                    n .frontFace = (tmpVoxel.active==0);

                    GetBuildNode(tmpVoxel,x,y,z-1);
                    n .backFace = (tmpVoxel.active==0);
                }
                break;
            }
            case 1: //xz slices
            {
                loop(y,16) loop(x,16)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpVoxel,x,z,y);
                    n = (tmpVoxel.active==1);

                    GetBuildNode(tmpVoxel,x,z+1,y);
                    n.frontFace = (tmpVoxel.active==0);

                    GetBuildNode(tmpVoxel,x,z-1,y);
                    n.backFace = (tmpVoxel.active==0);
                }
                break;
            }
            case 2: //yz slices
            {
                loop(y,16) loop(x,16)
                {
                    MaskNode & n = mask[x][y];

                    GetBuildNode(tmpVoxel,z,y,x);
                    n = (tmpVoxel.active==1);

                    GetBuildNode(tmpVoxel,z+1,y,x);
                    n.frontFace = (tmpVoxel.active==0);

                    GetBuildNode(tmpVoxel,z-1,y,x);
                    n.backFace = (tmpVoxel.active==0);
                }
                break;
            }
            default:
                break;
            }

            qstart.x = 0;
            qstart.y = 0;

            loop(y, 16)
            {
                loop(x, 16)
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
                                face[3]=u8vec3(qstart.x,             qstart.y,           z)+offset;
                                face[2]=u8vec3(qstart.x+qdims.x,     qstart.y,           z)+offset;
                                face[1]=u8vec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z)+offset;
                                face[0]=u8vec3(qstart.x,             qstart.y+qdims.y,   z)+offset;
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[0]=u8vec3(qbstart.x,            qbstart.y,              z+1)+offset;
                                face[1]=u8vec3(qbstart.x+qbdims.x,   qbstart.y,              z+1)+offset;
                                face[2]=u8vec3(qbstart.x+qbdims.x,   qbstart.y+qbdims.y,     z+1)+offset;
                                face[3]=u8vec3(qbstart.x,            qbstart.y+qbdims.y,     z+1)+offset;
                                AddQuadToMesh(face);
                                faceCount++;
                            }
                            break;
                        }
                        case 1: //xz
                        {
                            if(mn.backFace)
                            {
                                face[0]=u8vec3(qstart.x,         z,                  qstart.y)+offset;
                                face[1]=u8vec3(qstart.x+qdims.x, z,                  qstart.y)+offset;
                                face[2]=u8vec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y)+offset;
                                face[3]=u8vec3(qstart.x,         z,                  qstart.y+qdims.y)+offset;
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=u8vec3(qbstart.x,         z+1,                qbstart.y)+offset;
                                face[2]=u8vec3(qbstart.x+qbdims.x, z+1,                qbstart.y)+offset;
                                face[1]=u8vec3(qbstart.x+qbdims.x, z+1,                qbstart.y+qbdims.y)+offset;
                                face[0]=u8vec3(qbstart.x,         z+1,                qbstart.y+qbdims.y)+offset;
                                AddQuadToMesh(face);
                                faceCount++;
                            }
                            break;
                        }
                        case 2: //yz
                        {
                            if(mn.backFace)
                            {
                                face[0]=u8vec3(z,                qstart.y,           qstart.x)+offset;
                                face[1]=u8vec3(z,                qstart.y,           qstart.x+qdims.x)+offset;
                                face[2]=u8vec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x)+offset;
                                face[3]=u8vec3(z,                qstart.y+qdims.y,   qstart.x)+offset;
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=u8vec3(z+1,              qbstart.y,           qbstart.x)+offset;
                                face[2]=u8vec3(z+1,              qbstart.y,           qbstart.x+qbdims.x)+offset;
                                face[1]=u8vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x+qbdims.x)+offset;
                                face[0]=u8vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x)+offset;
                                AddQuadToMesh(face);
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

void VoxelMesh::AddQuadToMesh(const u8vec3 * face)
{
    BufferObject<u8vec3> *vbo = (BufferObject<u8vec3> *) m_mesh->buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) m_mesh->buffers[Mesh::INDICES];
    BufferObject<u8vec4> *cbo = (BufferObject<u8vec4> *) m_mesh->buffers[Mesh::COLOR];

    uint32_t indicesStart = vbo->data.size();
    u8vec4 color((rand()%256)/255.0f,(rand()%256),(rand()%256)/255.0f,1.f);

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

void VoxelMesh::CreateVox(uint8_t x, uint8_t y, uint8_t z)
{
    Voxel a;
    a.active=true;
    a.type=0;
    m_vox[x][y][z]=a;
}

void VoxelMesh::CreateVoxel(uint8_t x, uint8_t y, uint8_t z, uint32_t sides, u8vec4 color)
{
    uint8_t BLOCK_RENDER_SIZE=1;
    /// - - +
    u8vec3 p1(x, y, z+BLOCK_RENDER_SIZE);
    /// + - +
    u8vec3 p2(x+BLOCK_RENDER_SIZE, y, z+BLOCK_RENDER_SIZE);
    /// + + +
    u8vec3 p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// - + +
    u8vec3 p4(x, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);

    /// - - -
    u8vec3 p5(x, y, z);
    /// + - -
    u8vec3 p6(x+BLOCK_RENDER_SIZE, y, z);
    /// + + -
    u8vec3 p7(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z);
    /// - + -
    u8vec3 p8(x, y+BLOCK_RENDER_SIZE, z);

    m_posBuf->data.push_back(p1);
    m_posBuf->data.push_back(p2);
    m_posBuf->data.push_back(p3);
    m_posBuf->data.push_back(p4);
    m_posBuf->data.push_back(p5);
    m_posBuf->data.push_back(p6);
    m_posBuf->data.push_back(p7);
    m_posBuf->data.push_back(p8);

    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);


    // Front
    if(CheckBit(sides,EBS_FRONT))
    {
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+2);

        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+0);

        m_indexTrack+=6;
    }

    // Top
    if(CheckBit(sides,EBS_TOP))
    {
        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+6);

        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+3);

        m_indexTrack+=6;
    }

    // Back
    if(CheckBit(sides,EBS_BACK))
    {
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+5);

        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+7);

        m_indexTrack+=6;
    }

    // Bottom
    if(CheckBit(sides,EBS_BOTTOM))
    {
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+1);

        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+4);

        m_indexTrack+=6;
    }

    // Left
    if(CheckBit(sides,EBS_LEFT))
    {
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+3);

        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+4);

        m_indexTrack+=6;
    }

    // Right
    if(CheckBit(sides,EBS_RIGHT))
    {
        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+6);

        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+1);

        m_indexTrack+=6;
    }
    m_dirty=true;
    m_vertexTrack+=8;
}

