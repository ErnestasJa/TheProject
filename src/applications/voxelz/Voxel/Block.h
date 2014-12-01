#ifndef BLOCK_H
#define BLOCK_H

#include "Voxel.h"

enum EBlockType
{
    EBT_AIR,

    EBT_STONE,
    EBT_DIRT,
    EBT_GRASS,
    EBT_SAND,
    EBT_WATER,
    EBT_WOOD,
    EBT_LEAF,
    EBT_VOIDROCK,

    EBT_COUNT
};

struct Block: public Voxel
{
    Block()
    {
        active=false;
        type=EBT_AIR;
        color=u8vec4(255);
    }
    Block(const Block& other) = delete;
    Block& operator = (const Block&) = delete;
    uint32_t type;
};

#endif // BLOCK_H
