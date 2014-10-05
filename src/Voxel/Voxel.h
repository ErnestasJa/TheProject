#ifndef VOXEL_H_INCLUDED
#define VOXEL_H_INCLUDED

struct Voxel
{
    Voxel()
    {
        active=false;
        type=0;
        color=u8vec4(255);
    }
    uint8_t active:1;
    uint8_t type:1;
    u8vec4 color;
};

#endif // VOXEL_H_INCLUDED
