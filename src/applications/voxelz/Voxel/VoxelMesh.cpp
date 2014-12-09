#include "Precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "Chunk.h"
#include "VoxelMesh.h"
#include "utility/Timer.h"

Voxel VoxelMesh::EMPTY_VOXEL=Voxel();

VoxelMesh::VoxelMesh(uint32_t size)
{
    m_vox.reserve(size*size*size);

    mVecTrack=0;
    mIndexTrack=0;
    m_size=size;
    m_dirty=false;

    buffers[Mesh::POSITION] = new BufferObject<u8vec3>();
    buffers[Mesh::COLOR] = new BufferObject<u8vec4>();
    buffers[Mesh::INDICES] = new IndexBufferObject<uint32_t>();

    ((BufferObject<u8vec3> *) buffers[Mesh::POSITION])->data.reserve((uint32_t)glm::pow(size/2.f,3.f));
    ((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data.reserve((uint32_t)glm::pow(size/2.f,3.f));
    ((BufferObject<uint32_t> *) buffers[Mesh::INDICES])->data.reserve((uint32_t)glm::pow(size/2.f,3.f)*36);

    m_faceCount=0;
    m_empty=true;

    Init();
}

VoxelMesh::~VoxelMesh()
{
    Cleanup();
    m_vox.clear();
}

bool VoxelMesh::Empty()
{
    return m_empty;
}

void VoxelMesh::Render(bool wireframe)
{
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    if(m_dirty)
        Rebuild();

    if(!Empty()&&!m_dirty)
    {
        Mesh::Render();
    }


    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

void VoxelMesh::Cleanup()
{
    mVecTrack=0;
    mIndexTrack=0;
    if(!Empty())
    {
        m_vox.clear();

        ((BufferObject<u8vec3> *) buffers[Mesh::POSITION])->data.clear();
        ((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data.clear();
        ((BufferObject<uint32_t> *) buffers[Mesh::INDICES])->data.clear();

        m_empty=true;
    }
}

void VoxelMesh::ClearBuffers()
{
    ((BufferObject<u8vec3> *) buffers[Mesh::POSITION])->data.clear();
    ((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data.clear();
}

void VoxelMesh::UpdateMesh()
{
    if(!Empty())
    {
        Timer a(0);
        a.tick();
        GreedyBuild();
        a.tick();
        //printf("Greedy took: %u ms\n",a.get_delta_time());

        a.tick();
        UploadBuffers();
        ClearBuffers();
        a.tick();
        //printf("Upload took: %u ms\n",a.get_delta_time());

        a.tick();
        RecalculateAABB<u8vec3>();
        a.tick();
        //printf("AABB Recalculate took: %u ms\n",a.get_delta_time());
    }
}

const Voxel & VoxelMesh::GetVoxel(int32_t x,int32_t y, int32_t z)
{
    if(x>m_size-1 || x<0 || y>m_size-1 || y<0 || z>m_size-1 || z<0)
    {
        return VoxelMesh::EMPTY_VOXEL;
    }
    else
    {
        return m_vox[x+y*m_size+z*m_size*m_size];
    }
}

void VoxelMesh::clear_mask(MaskNode **mask)
{
    loop(i,m_size) loop(j,m_size)
    {
        mask[i][j].exists = false;
    }
}

void VoxelMesh::clear_mask_ranged(MaskNode **mask,int sx,int sy,int ex,int ey)
{
    for(int i=sx; i<ex; i++) for(int j=sy; j<ey; j++)
    {
        mask[i][j].exists = false;
    }
}

uint32_t VoxelMesh::length(uint32_t x, uint32_t y, MaskNode **mask)
{
    for(uint32_t i = x; i < m_size; i++)
        if(mask[i][y].color!=mask[x][y].color || !mask[i][y].exists)
            return i-x;

    return m_size-x;
}

uint32_t VoxelMesh::height(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < m_size; i++)
        if(length(x,i,mask)==len&&mask[x][i].color==mask[x][y].color)
            h++;
        else
            break;

    return h;
}

void VoxelMesh::GreedyBuild()
{
    MaskNode **mask=new MaskNode*[m_size];
    loop(i,m_size) mask[i]=new MaskNode[m_size];

    u8vec3 face[4];

    glm::ivec2 qstart, qdims;

    uint32_t faceCount = 0;

    for(uint32_t dim=0; dim<6; dim++)
    {
        clear_mask(mask);
        loopi(z, m_size)
        {
            switch(dim)
            {
            case 0: //z-
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(x,y,z);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {

                        const Voxel& tmpVoxel=GetVoxel(x,y,z-1);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            case 1: //z+
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(x,y,z);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {
                        const Voxel& tmpVoxel=GetVoxel(x,y,z+1);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            case 2: //y-
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(x,z,y);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {
                        const Voxel& tmpVoxel=GetVoxel(x,z+1,y);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            case 3: //y+
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(x,z,y);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {
                        const Voxel& tmpVoxel=GetVoxel(x,z-1,y);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            case 4: //x+
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(z,y,x);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {
                        const Voxel& tmpVoxel=GetVoxel(z+1,y,x);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            case 5: //x-
            {
                loopi(y,m_size) loopi(x,m_size)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVoxel=GetVoxel(z,y,x);
                    n.color=tmpVoxel.color;
                    if(tmpVoxel.active)
                    {
                        const Voxel& tmpVoxel=GetVoxel(z-1,y,x);
                        n.exists = !(tmpVoxel.active==1);
                    }
                }
                break;
            }
            default:
                break;
            }

            loop(y, m_size)
            {
                loop(x, m_size)
                {
                    MaskNode& mn = mask[x][y];
                    if(mn)
                    {
                        /*
                        *    We found our victim, lets find out how fat he is.
                        */
                        qstart.x=x;
                        qstart.y=y;
                        qdims.x =length(x,y,mask);
                        qdims.y =height(x,y,qdims.x,mask);

                        switch(dim)
                        {
                        case 0: //z+
                        {
                            face[3]=u8vec3(qstart.x,            qstart.y,              z);
                            face[2]=u8vec3(qstart.x+qdims.x,   qstart.y,              z);
                            face[1]=u8vec3(qstart.x+qdims.x,   qstart.y+qdims.y,     z);
                            face[0]=u8vec3(qstart.x,            qstart.y+qdims.y,     z);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 1: //z-
                        {
                            face[0]=u8vec3(qstart.x,             qstart.y,           z+1);
                            face[1]=u8vec3(qstart.x+qdims.x,     qstart.y,           z+1);
                            face[2]=u8vec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z+1);
                            face[3]=u8vec3(qstart.x,             qstart.y+qdims.y,   z+1);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 2: //y+
                        {
                            face[3]=u8vec3(qstart.x,         z+1,                qstart.y);
                            face[2]=u8vec3(qstart.x+qdims.x, z+1,                qstart.y);
                            face[1]=u8vec3(qstart.x+qdims.x, z+1,                qstart.y+qdims.y);
                            face[0]=u8vec3(qstart.x,         z+1,                qstart.y+qdims.y);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 3: //y-
                        {
                            face[0]=u8vec3(qstart.x,         z,                  qstart.y);
                            face[1]=u8vec3(qstart.x+qdims.x, z,                  qstart.y);
                            face[2]=u8vec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y);
                            face[3]=u8vec3(qstart.x,         z,                  qstart.y+qdims.y);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 4: //x+
                        {
                            face[3]=u8vec3(z+1,              qstart.y,           qstart.x);
                            face[2]=u8vec3(z+1,              qstart.y,           qstart.x+qdims.x);
                            face[1]=u8vec3(z+1,              qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[0]=u8vec3(z+1,              qstart.y+qdims.y,   qstart.x);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 5: //x-
                        {
                            face[0]=u8vec3(z,                qstart.y,           qstart.x);
                            face[1]=u8vec3(z,                qstart.y,           qstart.x+qdims.x);
                            face[2]=u8vec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[3]=u8vec3(z,                qstart.y+qdims.y,   qstart.x);
                            AddQuadToMesh(face,mn.color);
                            faceCount++;
                            break;
                        }
                        default:
                            break;
                        }
                        clear_mask_ranged(mask,qstart.x,qstart.y,qstart.x+qdims.x,qstart.y+qdims.y);
                    }
                }
            }
        }
    }

    loop(i,m_size) delete mask[i];
    delete mask;

    m_faceCount=faceCount*2;

    if(m_faceCount==0)
        m_empty=true;
}

void VoxelMesh::AddQuadToMesh(const u8vec3 * face, const intRGBA &col)
{
    BufferObject<u8vec3> *vbo = (BufferObject<u8vec3> *) buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) buffers[Mesh::INDICES];
    BufferObject<u8vec4> *cbo = (BufferObject<u8vec4> *) buffers[Mesh::COLOR];

    uint32_t indexStart=vbo->data.size();

    vbo->data.push_back(face[0]);
    vbo->data.push_back(face[1]);
    vbo->data.push_back(face[2]);
    vbo->data.push_back(face[3]);

    u8vec4 _col=IntRGBAToVecRGBA(col);

    cbo->data.push_back(_col);
    cbo->data.push_back(_col);
    cbo->data.push_back(_col);
    cbo->data.push_back(_col);

    ibo->data.push_back(indexStart);
    ibo->data.push_back(indexStart+2);
    ibo->data.push_back(indexStart+3);

    ibo->data.push_back(indexStart);
    ibo->data.push_back(indexStart+1);
    ibo->data.push_back(indexStart+2);
}

void VoxelMesh::CreateVox(int32_t x, int32_t y, int32_t z, const intRGBA &col)
{
    Voxel vox;
    vox.active=true;
    vox.color=col;
    if(m_empty)
        m_empty=false;
    m_vox[x+y*m_size+z*m_size*m_size]=vox;
}

void VoxelMesh::RemoveVox(int32_t x, int32_t y, int32_t z)
{
    m_vox[x+y*m_size+z*m_size*m_size]=EMPTY_VOXEL;
}

uint32_t VoxelMesh::GetFaceCount()
{
    return m_faceCount;
}

