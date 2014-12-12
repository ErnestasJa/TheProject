#include "Precomp.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "utility/SimplexNoise.h"
#include "utility/Timer.h"
#include <boost/foreach.hpp>

#define WORLD_HEIGHT 256

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
    int cunt=0;
    for(auto a:m_chunks)
    {
        cunt++;
        delete a.second;
    }
    printf("Cunt is %d\n",cunt);
    m_chunks.clear();
}

static void PlaceTrees(ChunkMap cmap)
{

}

void ChunkManager::Generate()
{
#define GEN_NEW
#ifdef GEN_FILE
    char * buf;
    uint32_t len;
    len=helpers::read("res/de_nuke.bvox",buf);
    uint32_t * data = (uint32_t*)((void*)&buf[0]);

    uint32_t voxel_count = data[0];
    data++;

    std::cout << "File len: " << len << std::endl;
    std::cout << "Voxel count: " << voxel_count << std::endl;

    for(int i = 0; i < voxel_count; i++)
    {
        uint32_t x = data[0], y = data[1], z = data[2];
        SetBlock(glm::vec3(x,y,z),EBT_WATER,true);
        data+=3;
    }

    delete[] buf;
#endif //GEN_FILE

#ifdef GEN_NOISE
    int testsize=256;
    int testheight=128;

    for(int x=-testsize; x<testsize; x++)
    {
        for(int z=-testsize; z<testsize; z++)
        {
            float h=scaled_raw_noise_2d(5,testheight,x/256.f,z/256.f);
            for(int y=0; y<testheight; y++)
            {
                if(y==0)
                {
                    SetBlock(glm::vec3(x,y,z),EBT_VOIDROCK,true);
                    continue;
                }
                else if(y==(int)h)
                {
                    float b=scaled_raw_noise_3d(0,testheight,x/256.f,y/256.f,z/256.f);
                    if(b*(rand()%5+1)<32)
                    {
                        SetBlock(glm::vec3(x,y,z),EBT_SAND,true);
                        continue;
                    }
                    SetBlock(glm::vec3(x,y,z),EBT_GRASS,true);
                    continue;
                }
                else if(y>h)
                {
                    if(y<60)
                    {
                        SetBlock(glm::vec3(x,y,z),EBT_WATER,false);
                        continue;
                    }
                    else if(GetBlock(glm::vec3(x, y - 1, z)).type == EBT_GRASS && (rand() & 0xff) == 0)
                    {
                        // Trunk
                        h = (rand() & 0x3) + 3;
                        for(int i = 0; i < h; i++)
                            SetBlock(glm::vec3(x, y + i, z), EBT_WOOD, true);

                        // Leaves
                        for(int ix = -3; ix <= 3; ix++)
                        {
                            for(int iy = -1; iy <= 1; iy++)
                            {
                                for(int iz = -3; iz <= 3; iz++)
                                {
                                    if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !GetBlock(glm::vec3(x + ix, y + h + iy, z + iz)).active)
                                        SetBlock(glm::vec3(x + ix, y + h + iy, z + iz), EBT_LEAF, true);
                                }
                            }
                        }
                    }
                    break;
                }
                else
                {
                    SetBlock(glm::vec3(x,y,z),EBT_STONE,true);
                }
            }
        }
    }
//    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
//    {
//        SetChunkNeighbours(a.second,a.first);
//    }
#endif // GEN_NOISE

#ifdef GEN_SINGLE
    AddChunk(glm::vec3(0));
    m_chunks[glm::vec3(0)]->Fill();
#endif // GEN_SINGLE

#ifdef GEN_FILL
    loop(i,16)
    loop(j,8)
    loop(k,16)
    {
        AddChunk(glm::vec3(i,j,k))->Fill();
    }
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        SetChunkNeighbours(a.second,a.first);
    }
#endif

#ifdef GEN_NEW
    loop(i,32)
    loop(j,16)
    loop(k,32)
    {
        ChunkPtr a=AddChunk(glm::ivec3(i,j,k));
        NoiseChunk(a);
    }
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        SetChunkNeighbours(a.second,a.first);
    }
#endif

}

void ChunkManager::RemoveChunk(const glm::ivec3& pos)
{
    //printf("I am being used by %d referencees\n",m_chunks[pos].use_count());

    if(m_chunks[pos]->leftN!=nullptr) m_chunks[pos]->leftN->rightN=nullptr;
    if(m_chunks[pos]->rightN!=nullptr) m_chunks[pos]->rightN->leftN=nullptr;

    if(m_chunks[pos]->botN!=nullptr) m_chunks[pos]->botN->topN=nullptr;
    if(m_chunks[pos]->topN!=nullptr) m_chunks[pos]->topN->botN=nullptr;

    if(m_chunks[pos]->backN!=nullptr) m_chunks[pos]->backN->frontN=nullptr;
    if(m_chunks[pos]->frontN!=nullptr) m_chunks[pos]->frontN->backN=nullptr;

    //printf("Now I am being used by %d referencees\n",m_chunks[pos].use_count());


    if(m_chunks.count(pos)!=0)
    {
        Chunk* temp=m_chunks[pos];
        m_chunks.erase(pos);
        delete temp;
    }
}

static const char* v3str(const glm::vec3 & vec)
{
    char buf[256];
    sprintf(buf,"vec3(%f %f %f)",vec.x,vec.y,vec.z);
    return std::string(buf).c_str();
}

void ChunkManager::NoiseChunk(ChunkPtr chunkToNoise)
{
    glm::ivec3 cpos=chunkToNoise->GetPosition();
    for(int x=0; x<16; x++)
    {
        for(int z=0; z<16; z++)
        {
            float h=scaled_raw_noise_2d(5,WORLD_HEIGHT/2,(cpos.x+x)/256.f,(cpos.z+z)/256.f);
            for(int y=0; y<16; y++)
            {
                if(cpos.y+y==0)
                {
                    chunkToNoise->Set(x,y,z,EBT_VOIDROCK,true);
                    continue;
                }
                else if(cpos.y+y==(int)h)
                {
                    float b=scaled_raw_noise_3d(0,WORLD_HEIGHT/2,(cpos.x+x)/256.f,(cpos.y+y)/256.f,(cpos.z+z)/256.f);
                    if(b*(rand()%5+1)<32)
                    {
                        chunkToNoise->Set(x,y,z,EBT_SAND,true);
                        continue;
                    }
                    chunkToNoise->Set(x,y,z,EBT_GRASS,true);
                    continue;
                }
                else if(cpos.y+y>h)
                {
                    if(cpos.y+y<60)
                    {
                        chunkToNoise->Set(x,y,z,EBT_WATER,true);
                        continue;
                    }
                    break;
                }
                else
                {
                    chunkToNoise->Set(x,y,z,EBT_STONE,true);
                }
            }
        }
    }
}

void ChunkManager::Explode(const glm::ivec3 &pos,float power)
{
    glm::vec3 posf=glm::vec3(pos);

    glm::vec3 startpos=posf-power;
    glm::vec3 endpos=posf+power;

    AABB ab=AABB(posf,glm::vec3(power/2.0f)); //+(CHUNK_SIZEF*((glm::vec3)a.first))
    printf("Explosion AABB size %f %f %f\n",ab.GetHalfSize().x*2,ab.GetHalfSize().y*2,ab.GetHalfSize().z*2);

    std::list<ChunkPtr> exploded;
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        a.second->aabb.Translate(CHUNK_SIZEF*glm::vec3(a.first));
        if(ab.IntersectsWith(a.second->aabb))
            exploded.push_back(a.second);
    }

    printf("BOOM! Position:%s Startbound:%s Endbound:%s \n",v3str(posf),v3str(startpos),v3str(endpos));

    for(int x=startpos.x; x<endpos.x; x++)
    {
        for(int y=startpos.y; y<endpos.y; y++)
        {
            for(int z=startpos.z; z<endpos.z; z++)
            {
                if (sqrt((float) glm::pow((posf.x-x),2.f) + glm::pow((posf.y-y),2.f) + glm::pow((posf.z-z),2.f)) <= power/2)
                {
                    SetBlock(glm::ivec3(x,y,z),EBT_AIR,false);
                }
            }
        }
    }
    printf("%u chunks updated.\n",exploded.size());
    if(exploded.size()>0)
        BOOST_FOREACH(std::list<ChunkPtr>::value_type a,exploded)
    {
        a->Rebuild();
    }
}

void ChunkManager::SetBlock(const glm::ivec3 &pos,EBlockType type,bool active)
{
    glm::ivec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);

    if(m_chunks.count(chunkCoords)!=0)
    {
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
    else
    {
        AddChunk(chunkCoords);
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

void ChunkManager::SetChunkNeighbours(ChunkPtr chunk,const glm::ivec3 &pos)
{
    glm::ivec3 posXNeg(pos.x-1,pos.y,pos.z),posXPos(pos.x+1,pos.y,pos.z),posYNeg(pos.x,pos.y-1,pos.z),posYPos(pos.x,pos.y+1,pos.z),posZNeg(pos.x,pos.y,pos.z-1),posZPos(pos.x,pos.y,pos.z+1);

    if(m_chunks.count(posXNeg)!=0) m_chunks[pos]->leftN=m_chunks[posXNeg];
    if(m_chunks.count(posXPos)!=0) m_chunks[pos]->rightN=m_chunks[posXPos];

    if(m_chunks.count(posYNeg)!=0) m_chunks[pos]->botN=m_chunks[posYNeg];
    if(m_chunks.count(posYPos)!=0) m_chunks[pos]->topN=m_chunks[posYPos];

    if(m_chunks.count(posZNeg)!=0) m_chunks[pos]->backN=m_chunks[posZNeg];
    if(m_chunks.count(posZPos)!=0) m_chunks[pos]->frontN=m_chunks[posZPos];
}

//! pos: in CHUNK coordinates
ChunkPtr ChunkManager::AddChunk(const glm::ivec3 &pos)
{
    if(m_chunks.count(pos)!=0)
    {
        return m_chunks[pos];
    }
    else
    {
        m_chunks[pos]=new Chunk(this,ChunkToWorldCoords(pos));
        //SetChunkNeighbours(m_chunks[pos],pos);
        return m_chunks[pos];
    }
}

//! pos: in WORLD coordinates
ChunkPtr ChunkManager::AddChunkWorld(const glm::ivec3 &pos)
{
    glm::ivec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
    {
        return m_chunks[chunkCoords];
    }
    else
    {
        m_chunks[chunkCoords]=new Chunk(this,ChunkToWorldCoords(chunkCoords));
        return m_chunks[chunkCoords];
    }
}

//! pos: in CHUNK coordinates
ChunkPtr ChunkManager::GetChunk(const glm::ivec3 &pos)
{
    if(m_chunks.count(pos)!=0)
        return m_chunks[pos];
    else
        return NullChunk;
}

//! pos: in WORLD coordinates
ChunkPtr ChunkManager::GetChunkWorld(const glm::ivec3 &pos)
{
    glm::ivec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
        return m_chunks[chunkCoords];
    else
        return NullChunk;
}

const Block &ChunkManager::GetBlock(const glm::ivec3 &pos)
{
    glm::ivec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
        return m_chunks[chunkCoords]->Get(voxelCoords.x,voxelCoords.y,voxelCoords.z);
    else
        return Chunk::EMPTY_BLOCK;
}

void ChunkManager::Render(Camera *cam,ShaderPtr vsh,bool wireframe)
{
    glm::mat4 Model;
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        //if(!a.second->Empty())
        {
            glm::vec3 pos=glm::vec3(a.first)*CHUNK_SIZEF;

            Model = glm::mat4(1.0f);
            Model = glm::translate(Model,pos);
            if(vsh->getparam("M")!=-1) MVar<glm::mat4>(vsh->getparam("M"), "M", Model).Set();
            glm::mat4 MVP=cam->GetProjectionMat()*cam->GetViewMat()*Model;
            glm::mat3 normMatrix = glm::transpose(glm::inverse(glm::mat3(cam->GetViewMat()*Model)));
            if(vsh->getparam("normMatrix")!=-1) MVar<glm::mat3>(vsh->getparam("normMatrix"), "normMatrix", normMatrix).Set();
            if(vsh->getparam("MVP")!=-1) MVar<glm::mat4>(vsh->getparam("MVP"), "MVP", MVP).Set();
            a.second->Render();
        }
    }
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

void ChunkManager::Clear()
{
//    for(boost::unordered_map<glm::vec3,Chunk*,chunkhasher>::iterator it=m_chunks.begin(); it!=m_chunks.end();)
//    {
//        m_chunks.erase(it++);
//    }
    m_chunks.clear();
}

uint32_t ChunkManager::GetChunkCount()
{
    return m_chunks.size();
}

uint32_t ChunkManager::GetTotalBlocks()
{
    uint32_t ret=0;
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        ret+=a.second->GetBlockCount();
    }
    return ret;
}

uint32_t ChunkManager::GetTotalFaces()
{
    uint32_t totalfaces=0;
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        totalfaces+=a.second->GetFaceCount();
    }
    return totalfaces;
}

