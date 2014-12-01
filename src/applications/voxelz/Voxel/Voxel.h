#ifndef VOXEL_H_INCLUDED
#define VOXEL_H_INCLUDED

#include "VoxelTypes.h"

struct Voxel
{
    Voxel()
    {
        active=false;
        color=u8vec4(255);
    }

    Voxel(const Voxel& other) = delete;
    Voxel& operator= (const Voxel& other)
    {
        active=other.active;
        color=other.color;
        return *this;
    }

    bool active;
    u8vec4 color;
};

#endif // VOXEL_H_INCLUDED
