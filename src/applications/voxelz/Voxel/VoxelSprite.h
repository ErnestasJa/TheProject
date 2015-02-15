#ifndef VOXELSPRITE_H
#define VOXELSPRITE_H

#include "VoxelMesh.h"

class image;
class VoxelSprite:public VoxelMesh
{
    public:
        VoxelSprite(const u16vec3 &size);

        virtual ~VoxelSprite();

        static VoxelSprite* LoadFromImage(std::shared_ptr<image> img,uint32_t extrude=1,u8vec4 discardColor=u8vec4(0));
        static VoxelSprite* LoadFromImage(std::shared_ptr<image> img, std::shared_ptr<image> dispimg, uint32_t extrude=1,u8vec4 discardColor=u8vec4(0));

        void Rebuild();
    protected:
    private:
};

#endif // VOXELSPRITE_H
