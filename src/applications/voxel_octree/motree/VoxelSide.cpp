#include "Precomp.h"
#include "VoxelSide.h"


glm::ivec3 VoxelSideToPosition(VoxelSide side)
{
    glm::ivec3 pos;

    switch(side)
    {
    case TOP:
        pos.y=1;
        break;
    case BOTTOM:
        pos.y=-1;
        break;
    case RIGHT:
        pos.x=1;
        break;
    case LEFT:
        pos.x=-1;
        break;
    case FRONT:
        pos.z=1;
        break;
    case BACK:
        pos.z=-1;
        break;
    default:
        break;
    }

    return pos;
}