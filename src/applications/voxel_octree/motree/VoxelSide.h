#ifndef EVOXEL_SIDE_H
#define EVOXEL_SIDE_H

enum VoxelSide
{
    NONE = 0,
    ALL = bit<0>() | bit<1>() | bit<2>() | bit<3>() | bit<4>() | bit<5>(),
    TOP = bit<0>(),
    BOTTOM = bit<1>(),
    FRONT = bit<2>(),
    RIGHT = bit<3>(),
    BACK = bit<4>(),
    LEFT = bit<5>()
};

glm::ivec3 VoxelSideToPosition(VoxelSide side);

#endif