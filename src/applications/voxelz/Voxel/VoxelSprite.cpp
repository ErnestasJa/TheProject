#include "Precomp.h"
#include "VoxelSprite.h"

#include "resources/Image.h"

VoxelSprite::VoxelSprite(const u16vec3 &size):VoxelMesh(size)
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
    VoxelSprite *ret=new VoxelSprite(u16vec3(img->width,img->height,extrude));

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

VoxelSprite* VoxelSprite::LoadFromImage(std::shared_ptr<image> img, std::shared_ptr<image> dispimg, uint32_t extrude,u8vec4 discardColor)
{
    uint8_t dispExtrMax=0;
    loop(x,dispimg->width)
    loop(y,dispimg->height)
    if(dispimg->GetPixel(x,dispimg->height-1-y).x>dispExtrMax)
        dispExtrMax=dispimg->GetPixel(x,dispimg->height-1-y).x;


    VoxelSprite *ret=new VoxelSprite(u16vec3(img->width,img->height,dispExtrMax));

    loop(x,img->width)
    loop(y,img->height)
    {
        u8vec4 height=dispimg->GetPixel(x,dispimg->height-1-y);
        uint8_t testrusion=height.x/64;

        u8vec4 pixel=img->GetPixel(x,img->height-1-y);
        if(pixel.a==0 || pixel==discardColor)
            continue;
        else
        {
            loopi(z,testrusion+2)
            ret->CreateVox(x,y,z,VecRGBAToIntRGBA(pixel));
        }

    }
    ret->Rebuild();
    return ret;
}

