#include "Precomp.h"
#include "VoxelSprite.h"

#include "resources/Image.h"

VoxelSprite::VoxelSprite(uint32_t size):VoxelMesh(size)
{

}

VoxelSprite::~VoxelSprite()
{

}

void VoxelSprite::Rebuild()
{
    UpdateMesh();
    _dirty=false;
}

VoxelSprite* VoxelSprite::LoadFromImage(std::shared_ptr<image> img,uint32_t extrude,u8vec4 discardColor)
{
    VoxelSprite *ret=new VoxelSprite(std::max(img->width,img->height));

    loop(z,extrude)
    loop(x,img->width)
    loop(y,img->height)
    {
        u8vec4 pixel=img->GetPixel(x,img->height-1-y);
        if(pixel.a==0 || pixel==discardColor)
            continue;
        else
            ret->CreateVox(x,y,z,VecRGBAToIntRGBA(pixel));
    }
    ret->Rebuild();
    return ret;
}
