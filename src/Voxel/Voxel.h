#ifndef VOXEL_H_INCLUDED
#define VOXEL_H_INCLUDED

struct Voxel
{
    Voxel()
    {
        active=false;
        type=0;
    }
    bool active;
    uint8_t type;
};

#endif // VOXEL_H_INCLUDED
