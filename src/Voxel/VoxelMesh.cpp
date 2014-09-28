#include "precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "Chunk.h"
#include "VoxelMesh.h"

VoxelMesh::VoxelMesh(uint32_t size):m_vox(boost::extents[size][size][size])
{
    m_size=size;
    m_dirty=false;

    buffers[Mesh::POSITION] = new BufferObject<u8vec3>();
    buffers[Mesh::COLOR] = new BufferObject<u8vec4>();
    buffers[Mesh::INDICES] = new IndexBufferObject<uint32_t>();

    Init();
}

VoxelMesh::~VoxelMesh()
{
}

bool VoxelMesh::Empty()
{
    loop(x,m_size) loop(y,m_size) loop(z,m_size) if(m_vox[x][y][z].active){return false;}
    return true;
}

void VoxelMesh::Render()
{
    if(m_dirty)
        Rebuild();

    if(!Empty()&&!m_dirty)
        Mesh::Render();

}

void VoxelMesh::Cleanup()
{
    if(!Empty())
    {
        loop(x,m_size)
        loop(y,m_size)
        loop(z,m_size)
        m_vox[x][y][z].active=false;

        ((BufferObject<u8vec3> *) buffers[Mesh::POSITION])->data.clear();
        ((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data.clear();
        ((BufferObject<uint32_t> *) buffers[Mesh::INDICES])->data.clear();
    }
}

void VoxelMesh::UpdateMesh()
{
    if(!Empty())
    {
        GreedyBuild();

        UploadBuffers();
    }
}

void VoxelMesh::GetVoxel(Voxel &vox,int32_t x,int32_t y, int32_t z)
{
    if(x>m_size-1 || x<0 || y>m_size-1 || y<0 || z>m_size-1 || z<0)
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

void VoxelMesh::clear_mask(MaskNode **mask)
{
    loop(i,m_size) loop(j,m_size)
    {
        mask[i][j].frontFace = false;
        mask[i][j].backFace = false;
        mask[i][j].exists = false;
    }
}

uint32_t VoxelMesh::length(uint32_t x, uint32_t y, MaskNode **mask, bool front)
{
    for(uint32_t i = x; i < m_size; i++)
        if(!mask[i][y].exists || (front?mask[i][y].frontFace==false:mask[i][y].backFace==false))
            return i-x;

    return m_size-x;
}

uint32_t VoxelMesh::height(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask, bool front)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < m_size; i++)
        if(length(x,i,mask,front)==len)
            h++;
        else
            break;

    return h;
}

void VoxelMesh::GreedyBuild()
{
    MaskNode **mask=new MaskNode*[m_size];
    loop(i,m_size) mask[i]=new MaskNode[m_size];

    MaskNode mn;
    Voxel tmpVoxel;

    u8vec3 face[4];

    glm::ivec2 qstart, qdims, qbstart, qbdims;

    uint32_t faceCount = 0;

    for(uint32_t dim=0; dim<3; dim++)
    {
        clear_mask(mask);
        loopi(z, m_size)
        {
            switch(dim)
            {
            case 0: //xy slices
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    GetVoxel(tmpVoxel,x,y,z);
                    n = (tmpVoxel.active==1);

                    GetVoxel(tmpVoxel,x,y,z+1);
                    n .frontFace = (tmpVoxel.active==0);

                    GetVoxel(tmpVoxel,x,y,z-1);
                    n .backFace = (tmpVoxel.active==0);
                }
                break;
            }
            case 1: //xz slices
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    GetVoxel(tmpVoxel,x,z,y);
                    n = (tmpVoxel.active==1);

                    GetVoxel(tmpVoxel,x,z+1,y);
                    n.frontFace = (tmpVoxel.active==0);

                    GetVoxel(tmpVoxel,x,z-1,y);
                    n.backFace = (tmpVoxel.active==0);
                }
                break;
            }
            case 2: //yz slices
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    GetVoxel(tmpVoxel,z,y,x);
                    n = (tmpVoxel.active==1);

                    GetVoxel(tmpVoxel,z+1,y,x);
                    n.frontFace = (tmpVoxel.active==0);

                    GetVoxel(tmpVoxel,z-1,y,x);
                    n.backFace = (tmpVoxel.active==0);
                }
                break;
            }
            default:
                break;
            }

            qstart.x = 0;
            qstart.y = 0;

            loop(y, m_size)
            {
                loop(x, m_size)
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
                                face[3]=u8vec3(qstart.x,             qstart.y,           z);
                                face[2]=u8vec3(qstart.x+qdims.x,     qstart.y,           z);
                                face[1]=u8vec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z);
                                face[0]=u8vec3(qstart.x,             qstart.y+qdims.y,   z);
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[0]=u8vec3(qbstart.x,            qbstart.y,              z+1);
                                face[1]=u8vec3(qbstart.x+qbdims.x,   qbstart.y,              z+1);
                                face[2]=u8vec3(qbstart.x+qbdims.x,   qbstart.y+qbdims.y,     z+1);
                                face[3]=u8vec3(qbstart.x,            qbstart.y+qbdims.y,     z+1);
                                AddQuadToMesh(face);
                                faceCount++;
                            }
                            break;
                        }
                        case 1: //xz
                        {
                            if(mn.backFace)
                            {
                                face[0]=u8vec3(qstart.x,         z,                  qstart.y);
                                face[1]=u8vec3(qstart.x+qdims.x, z,                  qstart.y);
                                face[2]=u8vec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y);
                                face[3]=u8vec3(qstart.x,         z,                  qstart.y+qdims.y);
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=u8vec3(qbstart.x,         z+1,                qbstart.y);
                                face[2]=u8vec3(qbstart.x+qbdims.x, z+1,                qbstart.y);
                                face[1]=u8vec3(qbstart.x+qbdims.x, z+1,                qbstart.y+qbdims.y);
                                face[0]=u8vec3(qbstart.x,         z+1,                qbstart.y+qbdims.y);
                                AddQuadToMesh(face);
                                faceCount++;
                            }
                            break;
                        }
                        case 2: //yz
                        {
                            if(mn.backFace)
                            {
                                face[0]=u8vec3(z,                qstart.y,           qstart.x);
                                face[1]=u8vec3(z,                qstart.y,           qstart.x+qdims.x);
                                face[2]=u8vec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x);
                                face[3]=u8vec3(z,                qstart.y+qdims.y,   qstart.x);
                                AddQuadToMesh(face);
                                faceCount++;
                            }

                            if(mn.frontFace)
                            {
                                face[3]=u8vec3(z+1,              qbstart.y,           qbstart.x);
                                face[2]=u8vec3(z+1,              qbstart.y,           qbstart.x+qbdims.x);
                                face[1]=u8vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x+qbdims.x);
                                face[0]=u8vec3(z+1,              qbstart.y+qbdims.y,   qbstart.x);
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

    loop(i,m_size) delete mask[i];
    delete mask;

    std::cout << "Added " << faceCount*2 << " faces to mesh" << std::endl;
}

void VoxelMesh::AddQuadToMesh(const u8vec3 * face)
{
    BufferObject<u8vec3> *vbo = (BufferObject<u8vec3> *) buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) buffers[Mesh::INDICES];
    BufferObject<u8vec4> *cbo = (BufferObject<u8vec4> *) buffers[Mesh::COLOR];

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

void VoxelMesh::CreateVox(int32_t x, int32_t y, int32_t z)
{
    m_vox[x][y][z].active=true;
}

