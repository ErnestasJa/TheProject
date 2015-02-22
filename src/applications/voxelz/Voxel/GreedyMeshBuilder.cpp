#include "Precomp.h"

#include "VoxelTypes.h"
#include "GreedyMeshBuilder.h"
#include "Chunk.h"
#include "VoxelMesh.h"

template<>
void GreedyMeshBuilder::_clearMask(MaskNode **mask,uint32_t size)
{
    loop(i,size) loop(j,size)
    {
        mask[i][j].exists = false;
    }
}

template<>
void GreedyMeshBuilder::_clearMask(MaskNode **mask,u16vec3 size)
{
    loop(i,size.x) loop(j,size.y)
    {
        mask[i][j].exists = false;
    }
}

void GreedyMeshBuilder::_clearMaskRanged(MaskNode **mask,int sx,int sy,int ex,int ey)
{
    for(int i=sx; i<ex; i++) for(int j=sy; j<ey; j++)
        {
            mask[i][j].exists = false;
        }
}

template<>
uint32_t GreedyMeshBuilder::_quadLength(uint32_t x, uint32_t y, MaskNode **mask,uint32_t size)
{
    for(uint32_t i = x; i < size; i++)
        if(mask[i][y].color!=mask[x][y].color || !mask[i][y].exists)
            return i-x;

    return size-x;
}

template<>
uint32_t GreedyMeshBuilder::_quadLength(uint32_t x, uint32_t y, MaskNode **mask,u16vec3 size)
{
    for(uint32_t i = x; i < size.x; i++)
        if(mask[i][y].color!=mask[x][y].color || !mask[i][y].exists)
            return i-x;

    return size.x-x;
}

template<>
uint32_t GreedyMeshBuilder::_quadHeight(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask,uint32_t size)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < size; i++)
        if(_quadLength(x,i,mask,size)==len&&mask[x][i].color==mask[x][y].color)
            h++;
        else
            break;

    return h;
}

template<>
uint32_t GreedyMeshBuilder::_quadHeight(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask,u16vec3 size)
{
    uint32_t h = 0;

    for(uint32_t i = y; i < size.y; i++)
        if(_quadLength(x,i,mask,size)==len&&mask[x][i].color==mask[x][y].color)
            h++;
        else
            break;

    return h;
}

template<>
void GreedyMeshBuilder::AddQuadToMesh(ChunkPtr chk,const glm::ivec3 * face, const intRGBA & col)
{
    uint32_t indexStart=chk->offset+chk->meshData.positions.size();

    chk->meshData.positions.push_back((u16vec3)(chk->position+face[0]));
    chk->meshData.positions.push_back((u16vec3)(chk->position+face[1]));
    chk->meshData.positions.push_back((u16vec3)(chk->position+face[2]));
    chk->meshData.positions.push_back((u16vec3)(chk->position+face[3]));

    u8vec4 _col=IntRGBAToVecRGBA(col);

    chk->meshData.colors.push_back(_col);
    chk->meshData.colors.push_back(_col);
    chk->meshData.colors.push_back(_col);
    chk->meshData.colors.push_back(_col);

    chk->meshData.indices.push_back(indexStart);
    chk->meshData.indices.push_back(indexStart+2);
    chk->meshData.indices.push_back(indexStart+3);

    chk->meshData.indices.push_back(indexStart);
    chk->meshData.indices.push_back(indexStart+1);
    chk->meshData.indices.push_back(indexStart+2);
}

template<>
void GreedyMeshBuilder::AddQuadToMesh(ChunkPtr chk,const glm::ivec3 * face, const u16vec4& inds, const intRGBA & col)
{
    printf("Inds: %u,%u,%u,%u\n",inds[0],inds[1],inds[2],inds[3]);
    uint32_t indexStart=chk->offset+chk->meshData.positions.size();

    chk->meshData.positions[inds[0]]=(u16vec3)(chk->position+face[0]);
    chk->meshData.positions[inds[1]]=(u16vec3)(chk->position+face[1]);
    chk->meshData.positions[inds[2]]=(u16vec3)(chk->position+face[2]);
    chk->meshData.positions[inds[3]]=(u16vec3)(chk->position+face[3]);

    u8vec4 _col=IntRGBAToVecRGBA(col);

    chk->meshData.colors[inds[0]]=_col;
    chk->meshData.colors[inds[1]]=_col;
    chk->meshData.colors[inds[2]]=_col;
    chk->meshData.colors[inds[3]]=_col;

    chk->meshData.indices.push_back(chk->offset+inds[0]);
    chk->meshData.indices.push_back(chk->offset+inds[2]);
    chk->meshData.indices.push_back(chk->offset+inds[3]);

    chk->meshData.indices.push_back(chk->offset+inds[0]);
    chk->meshData.indices.push_back(chk->offset+inds[1]);
    chk->meshData.indices.push_back(chk->offset+inds[2]);
}

template<>
void GreedyMeshBuilder::AddQuadToMesh(VoxelMesh* vox,const glm::ivec3 * face, const intRGBA & col)
{
    BufferObject<glm::ivec3> *vbo = (BufferObject<glm::ivec3> *) vox->buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) vox->buffers[Mesh::INDICES];
    BufferObject<u8vec4> *cbo = (BufferObject<u8vec4> *) vox->buffers[Mesh::COLOR];

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

    //printf("\nQuad added: \n%s \n%s \n%s \n%s\n\n",GLMVec3ToStr(face[0]),GLMVec3ToStr(face[1]),GLMVec3ToStr(face[2]),GLMVec3ToStr(face[3]));
}

#define INDEX3D(x,y,z,size) (x+y*size+z*size*size)

template <>
void GreedyMeshBuilder::GreedyBuild(ChunkPtr chk)
{
    chk->meshData.Free();
    //chk->meshData.indices.clear();

    uint32_t size=CHUNK_SIZE;
    MaskNode **mask=new MaskNode*[size];
    loop(i,CHUNK_SIZE) mask[i]=new MaskNode[size];

    glm::ivec3 face[4];
    u16vec4 inds;

    glm::ivec2 qstart, qdims;

    uint32_t faceCount = 0;

    for(uint32_t dim=0; dim<6; dim++)
    {
        _clearMask(mask,size);
        loop(z, size)
        {
            switch(dim)
            {
            case 0: //z-
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(x,y,z);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {

                        const Block& tmpBlock=chk->ElementAt(x,y,z-1);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            case 1: //z+
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(x,y,z);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {
                        const Block& tmpBlock=chk->ElementAt(x,y,z+1);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            case 2: //y-
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(x,z,y);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {
                        const Block& tmpBlock=chk->ElementAt(x,z+1,y);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            case 3: //y+
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(x,z,y);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {
                        const Block& tmpBlock=chk->ElementAt(x,z-1,y);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            case 4: //x+
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(z,y,x);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {
                        const Block& tmpBlock=chk->ElementAt(z+1,y,x);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            case 5: //x-
            {
                loop(y,size) loop(x,size)
                {
                    MaskNode & n = mask[x][y];

                    const Block& tmpBlock=chk->ElementAt(z,y,x);
                    n.color=Chunk::getTypeCol(tmpBlock.type);
                    if(tmpBlock.active)
                    {
                        const Block& tmpBlock=chk->ElementAt(z-1,y,x);
                        n.exists = !(tmpBlock.active==1);
                    }
                }
                break;
            }
            default:
                break;
            }

            loop(y, size)
            {
                loop(x, size)
                {
                    MaskNode& mn = mask[x][y];
                    if(mn)
                    {
                        /*
                        *    We found our victim, lets find out how fat he is.
                        */
                        qstart.x=x;
                        qstart.y=y;
                        qdims.x =_quadLength(x,y,mask,size);
                        qdims.y =_quadHeight(x,y,qdims.x,mask,size);

                        switch(dim)
                        {
                        case 0: //z+
                        {
                            face[3]=glm::ivec3(qstart.x,            qstart.y,              z);
                            face[2]=glm::ivec3(qstart.x+qdims.x,   qstart.y,              z);
                            face[1]=glm::ivec3(qstart.x+qdims.x,   qstart.y+qdims.y,     z);
                            face[0]=glm::ivec3(qstart.x,            qstart.y+qdims.y,     z);

                            inds=u16vec4(INDEX3D(qstart.x,            qstart.y+qdims.y,     z,size),
                                         INDEX3D(qstart.x+qdims.x,   qstart.y+qdims.y,     z,size),
                                         INDEX3D(qstart.x+qdims.x,   qstart.y,              z,size),
                                         INDEX3D(qstart.x,            qstart.y,              z,size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 1: //z-
                        {
                            face[0]=glm::ivec3(qstart.x,             qstart.y,           z+1);
                            face[1]=glm::ivec3(qstart.x+qdims.x,     qstart.y,           z+1);
                            face[2]=glm::ivec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z+1);
                            face[3]=glm::ivec3(qstart.x,             qstart.y+qdims.y,   z+1);

                            inds=u16vec4(INDEX3D(qstart.x,          qstart.y,           z+1,size),
                                         INDEX3D(qstart.x+qdims.x,  qstart.y,           z+1,size),
                                         INDEX3D(qstart.x+qdims.x,  qstart.y+qdims.y,   z+1,size),
                                         INDEX3D(qstart.x,          qstart.y+qdims.y,   z+1,size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 2: //y+
                        {
                            face[3]=glm::ivec3(qstart.x,         z+1,                qstart.y);
                            face[2]=glm::ivec3(qstart.x+qdims.x, z+1,                qstart.y);
                            face[1]=glm::ivec3(qstart.x+qdims.x, z+1,                qstart.y+qdims.y);
                            face[0]=glm::ivec3(qstart.x,         z+1,                qstart.y+qdims.y);

                            inds=u16vec4(INDEX3D(qstart.x,         z+1,                qstart.y+qdims.y,           size),
                                         INDEX3D(qstart.x+qdims.x, z+1,                qstart.y+qdims.y,           size),
                                         INDEX3D(qstart.x+qdims.x, z+1,                qstart.y,   size),
                                         INDEX3D(qstart.x,         z+1,                qstart.y,   size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 3: //y-
                        {
                            face[0]=glm::ivec3(qstart.x,         z,                  qstart.y);
                            face[1]=glm::ivec3(qstart.x+qdims.x, z,                  qstart.y);
                            face[2]=glm::ivec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y);
                            face[3]=glm::ivec3(qstart.x,         z,                  qstart.y+qdims.y);

                            inds=u16vec4(INDEX3D(qstart.x,              z,  qstart.y,           size),
                                         INDEX3D(qstart.x+qdims.x,      z,  qstart.y,           size),
                                         INDEX3D(qstart.x+qdims.x,      z,  qstart.y+qdims.y,   size),
                                         INDEX3D(qstart.x,              z,  qstart.y+qdims.y,   size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 4: //x+
                        {
                            face[3]=glm::ivec3(z+1,              qstart.y,           qstart.x);
                            face[2]=glm::ivec3(z+1,              qstart.y,           qstart.x+qdims.x);
                            face[1]=glm::ivec3(z+1,              qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[0]=glm::ivec3(z+1,              qstart.y+qdims.y,   qstart.x);

                            inds=u16vec4(INDEX3D(z+1,              qstart.y+qdims.y,   qstart.x,           size),
                                         INDEX3D(z+1,              qstart.y+qdims.y,   qstart.x+qdims.x,   size),
                                         INDEX3D(z+1,              qstart.y,           qstart.x+qdims.x,   size),
                                         INDEX3D(z+1,              qstart.y,           qstart.x,           size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 5: //x-
                        {
                            face[0]=glm::ivec3(z,                qstart.y,           qstart.x);
                            face[1]=glm::ivec3(z,                qstart.y,           qstart.x+qdims.x);
                            face[2]=glm::ivec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[3]=glm::ivec3(z,                qstart.y+qdims.y,   qstart.x);

                            inds=u16vec4(INDEX3D(z,qstart.y,          qstart.x,           size),
                                         INDEX3D(z,qstart.y,          qstart.x+qdims.x,   size),
                                         INDEX3D(z,qstart.y+qdims.y,  qstart.x+qdims.x,   size),
                                         INDEX3D(z,qstart.y+qdims.y,  qstart.x,           size)
                                        );

                            AddQuadToMesh(chk,face,mn.color);
                            faceCount++;
                            break;
                        }
                        default:
                            break;
                        }
                        _clearMaskRanged(mask,qstart.x,qstart.y,qstart.x+qdims.x,qstart.y+qdims.y);
                    }
                }
            }
        }
    }

    loop(i,size) delete mask[i];
    delete mask;

    if(faceCount==0)
        chk->meshData.empty=true;
    else
        chk->meshData.empty=false;

    chk->built=true;


    uint32_t possize=chk->meshData.positions.size();
    uint32_t colsize=chk->meshData.colors.size();
    uint32_t indsize=chk->meshData.indices.size();

    //printf("Faces: %u\nChunk data: o %d p %d c %d i %d\n",faceCount,chk->offset,possize,colsize,indsize);

}

template <>
void GreedyMeshBuilder::GreedyBuild(VoxelMesh* vxm)
{
    u16vec3 m_size=vxm->_size;
    MaskNode **mask=new MaskNode*[m_size.x];
    loop(i,m_size.x) mask[i]=new MaskNode[m_size.y];

    glm::ivec3 face[4];

    glm::ivec2 qstart, qdims;

    uint32_t faceCount = 0;

    for(uint32_t dim=0; dim<6; dim++)
    {
        _clearMask(mask,m_size);
        loop(z, m_size.z)
        {
            switch(dim)
            {
            case 0: //z-
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(x,y,z);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(x,y,z-1);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            case 1: //z+
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(x,y,z);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(x,y,z+1);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            case 2: //y-
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(x,z,y);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(x,z+1,y);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            case 3: //y+
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(x,z,y);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(x,z-1,y);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            case 4: //x+
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(z,y,x);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(z+1,y,x);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            case 5: //x-
            {
                loop(y,m_size.y) loop(x,m_size.x)
                {
                    MaskNode & n = mask[x][y];

                    const Voxel& tmpVox=vxm->GetVoxel(z,y,x);
                    n.color=tmpVox.color;
                    if(tmpVox.active)
                    {
                        const Voxel& tmpVox=vxm->GetVoxel(z-1,y,x);
                        n.exists = !(tmpVox.active==1);
                    }
                }
                break;
            }
            default:
                break;
            }

            loop(y, m_size.y)
            {
                loop(x, m_size.x)
                {
                    MaskNode& mn = mask[x][y];
                    if(mn)
                    {
                        /*
                        *    We found our victim, lets find out how fat he is.
                        */
                        qstart.x=x;
                        qstart.y=y;
                        qdims.x =_quadLength(x,y,mask,m_size);
                        qdims.y =_quadHeight(x,y,qdims.x,mask,m_size);

                        switch(dim)
                        {
                        case 0: //z+
                        {
                            face[3]=glm::ivec3(qstart.x,            qstart.y,              z);
                            face[2]=glm::ivec3(qstart.x+qdims.x,   qstart.y,              z);
                            face[1]=glm::ivec3(qstart.x+qdims.x,   qstart.y+qdims.y,     z);
                            face[0]=glm::ivec3(qstart.x,            qstart.y+qdims.y,     z);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 1: //z-
                        {
                            face[0]=glm::ivec3(qstart.x,             qstart.y,           z+1);
                            face[1]=glm::ivec3(qstart.x+qdims.x,     qstart.y,           z+1);
                            face[2]=glm::ivec3(qstart.x+qdims.x,     qstart.y+qdims.y,   z+1);
                            face[3]=glm::ivec3(qstart.x,             qstart.y+qdims.y,   z+1);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 2: //y+
                        {
                            face[3]=glm::ivec3(qstart.x,         z+1,                qstart.y);
                            face[2]=glm::ivec3(qstart.x+qdims.x, z+1,                qstart.y);
                            face[1]=glm::ivec3(qstart.x+qdims.x, z+1,                qstart.y+qdims.y);
                            face[0]=glm::ivec3(qstart.x,         z+1,                qstart.y+qdims.y);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 3: //y-
                        {
                            face[0]=glm::ivec3(qstart.x,         z,                  qstart.y);
                            face[1]=glm::ivec3(qstart.x+qdims.x, z,                  qstart.y);
                            face[2]=glm::ivec3(qstart.x+qdims.x, z,                  qstart.y+qdims.y);
                            face[3]=glm::ivec3(qstart.x,         z,                  qstart.y+qdims.y);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 4: //x+
                        {
                            face[3]=glm::ivec3(z+1,              qstart.y,           qstart.x);
                            face[2]=glm::ivec3(z+1,              qstart.y,           qstart.x+qdims.x);
                            face[1]=glm::ivec3(z+1,              qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[0]=glm::ivec3(z+1,              qstart.y+qdims.y,   qstart.x);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        case 5: //x-
                        {
                            face[0]=glm::ivec3(z,                qstart.y,           qstart.x);
                            face[1]=glm::ivec3(z,                qstart.y,           qstart.x+qdims.x);
                            face[2]=glm::ivec3(z,                qstart.y+qdims.y,   qstart.x+qdims.x);
                            face[3]=glm::ivec3(z,                qstart.y+qdims.y,   qstart.x);
                            AddQuadToMesh(vxm,face,mn.color);
                            faceCount++;
                            break;
                        }
                        default:
                            break;
                        }
                        _clearMaskRanged(mask,qstart.x,qstart.y,qstart.x+qdims.x,qstart.y+qdims.y);
                    }
                }
            }
        }
    }

    printf("%d\n",faceCount*2);

    BufferObject<glm::ivec3>* pb=(BufferObject<glm::ivec3>*)vxm->buffers[Mesh::POSITION];
    BufferObject<u8vec4>* cb=(BufferObject<u8vec4>*)vxm->buffers[Mesh::COLOR];
    IndexBufferObject<uint32_t>* ib=(IndexBufferObject<uint32_t>*)vxm->buffers[Mesh::INDICES];
    printf("Dataz %d %d %d\n",pb->data.size(),cb->data.size(),ib->data.size());

    loop(i,m_size.x) delete mask[i];
    delete mask;

    vxm->_dirty=false;
}
