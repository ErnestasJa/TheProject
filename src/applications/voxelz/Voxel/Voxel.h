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
    bool active;
    u8vec4 color;
};

#endif // VOXEL_H_INCLUDED
