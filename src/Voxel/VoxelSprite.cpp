#include "Precomp.h"
#include "VoxelSprite.h"

#include "resources/image.h"

VoxelSprite::VoxelSprite()
{

}

VoxelSprite::~VoxelSprite()
{

}

void VoxelSprite::Rebuild()
{
    UpdateMesh();
    m_dirty=false;
}

VoxelSprite* VoxelSprite::LoadFromImage(std::shared_ptr<image> img,uint32_t extrude,u8vec4 discardColor)
{
    VoxelSprite *ret=new VoxelSprite();
    loop(z,extrude)
    loop(x,img->width)
    loop(y,img->height)
    {
        u8vec4 pixel=img->GetPixel(x,img->height-1-y);
        if(pixel.a==0)
            continue;
        else
            ret->CreateVoxel(x,y,z,EBS_BACK|EBS_BOTTOM|EBS_FRONT|EBS_LEFT|EBS_RIGHT|EBS_TOP,pixel);
    }
    return ret;
}
