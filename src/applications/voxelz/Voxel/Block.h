#ifndef BLOCK_H
#define BLOCK_H

#include "Voxel.h"

enum EBlockType:uint32_t
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
    EBT_CLOUD,

    EBT_COUNT
};

struct Block
{
    Block()
    {
        active=false;
        type=EBT_AIR;
    }

    bool operator==(const Block& other) const
    {
        return this->active==other.active && this->type==other.type;
    }

    bool active;
    uint32_t type;
};

#endif // BLOCK_H
