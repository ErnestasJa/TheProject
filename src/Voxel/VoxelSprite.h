#ifndef VOXELSPRITE_H
#define VOXELSPRITE_H

#include "VoxelMesh.h"

class VoxelSprite:public VoxelMesh
{
    public:
        VoxelSprite();
        virtual ~VoxelSprite();

        //void LoadFromImage(std::shared_ptr<image> img);
    protected:
    private:
};

#endif // VOXELSPRITE_H
