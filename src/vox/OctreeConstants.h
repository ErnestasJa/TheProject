#ifndef OCTREE_CONSTANTS_H
#define OCTREE_CONSTANTS_H

static const uint8_t MAX_DEPTH_LEVEL = 10;
static const uint8_t OCTREE_DEPTH = MAX_DEPTH_LEVEL;
static const uint32_t DEPTH_HALF_TABLE[]= {0,1,2,4,8,16,32,64,128,256,512,1024};    ///HALF NODE COUNT AT LEVEL X
static const uint32_t SIZE_TABLE[]=       {1024,512,256,128,64,32,16,8,4,2,1};      ///NODE SIZE AT LEVEL X
static const uint32_t DEPTH_TABLE[]=      {2,4,8,16,32,64,128,256,512,1024};        ///NODE COUNT AT LEVEL X
static const uint32_t POSITION_MASK[]= {0xfffffe00,0xffffff00,0xffffff80,0xffffffc0,0xffffffe0,0xfffffff0,0xfffffff8,0xfffffffc,0xfffffffe,0xffffffff}; /// Don't question the hax
static const uint32_t CHUNK_MASK= ~0x7FFF;
static const uint32_t LOCAL_VOXEL_MASK= 0x7FFF;

#endif