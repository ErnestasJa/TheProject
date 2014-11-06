#include "Precomp.h"
#include "resources/Image.h"
#include "Texture.h"

uint32_t Texture::active_slot = 0;
uint32_t Texture::current = 0;


Texture::Texture()
{
    Id = -1;
    Type = GL_TEXTURE_2D;
}

Texture::~Texture()
{
    glDeleteTextures(1,&Id);
}

void Texture::Init(std::shared_ptr<image> img)
{
    glGenTextures(1, &Id);
    glBindTexture(Type,Id);

    glTexParameteri(Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    /// either use Texture with or without alpha channel
    switch(img->num_channels)
    {
    case 3:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        break;

    case 4:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;

    default:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    glTexImage2D(Type,0,GL_RGBA,img->width,img->height,0,img->num_channels==4?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,img->data);

    glGenerateMipmap(Type);

    glBindTexture(Type,current);
}

/**
 * @brief
 * @param clamp
 * bit0 = s to edge, bit1 = t to edge
 * @param filter
 * bit0 = linear, bit1 = mipmap
 */
void Texture::Init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h)
{
    Type = target;

    glGenTextures(1, &Id);
    glBindTexture(Type,Id);

    glTexParameteri(Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    /// either use Texture with or without alpha channel
    switch(image_format)
    {
    case GL_RED:
    case GL_RGB:
    case GL_BGR:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        break;

    case GL_RG:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        break;

    case GL_DEPTH_COMPONENT:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        break;

    default:
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;
    }

    uint32_t data_type = GL_UNSIGNED_BYTE;

    if(internal_format==GL_RGBA32F||internal_format==GL_DEPTH_COMPONENT)
        data_type = GL_FLOAT;

    glTexImage2D(Type,0,internal_format,w,h,0,image_format, data_type,data);

    //glinitMipmap(Type);

    glBindTexture(Type,current);
}

void Texture::SetFilters(FILTER_MIN fmin, FILTER_MAG fmag)
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glTexParameteri(Type, GL_TEXTURE_MAG_FILTER, fmag == FILTER_MAG_LINEAR ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(Type, GL_TEXTURE_MIN_FILTER, fmin == FILTER_MIN_LINEAR_MIPMAP ? GL_LINEAR_MIPMAP_LINEAR : (fmin==FILTER_MIN_NEAREST_MIPMAP ? GL_NEAREST_MIPMAP_NEAREST : (fmin==FILTER_MIN_LINEAR ? GL_LINEAR : GL_NEAREST)));

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::SetClampMode(Texture::CLAMP x, Texture::CLAMP y)
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glTexParameteri(Type, GL_TEXTURE_WRAP_S, x == CLAMP_EDGE ? GL_CLAMP_TO_EDGE : (x == CLAMP_BORDER ? GL_CLAMP_TO_BORDER : GL_REPEAT));
    glTexParameteri(Type, GL_TEXTURE_WRAP_T, y == CLAMP_EDGE ? GL_CLAMP_TO_EDGE : (y == CLAMP_BORDER ? GL_CLAMP_TO_BORDER : GL_REPEAT));

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::SetBorderColor(const glm::vec4 & color)
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,&color[0]);

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::InitMipmap()
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glGenerateMipmap(Type);

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::InitMipmap(uint32_t base, uint32_t max)
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glTexParameteri(Type,GL_TEXTURE_BASE_LEVEL,base);
    glTexParameteri(Type,GL_TEXTURE_MAX_LEVEL,max);
    glGenerateMipmap(Type);

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::UpdateMipmaps()
{
    if(current!=Id)
    glBindTexture(Type,Id);

    glGenerateMipmap(Type);

    if(current!=Id)
    glBindTexture(Type,current);
}

void Texture::Set(uint8_t slot)
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(Type,Id);

    active_slot = slot;
    current = Id;
}

void Texture::SetSubImage2D(const uint8_t * data,uint32_t xoffset,uint32_t yoffset,uint32_t width,uint32_t height)
{
    glActiveTexture(GL_TEXTURE0+active_slot);
    glBindTexture(Type,Id);

    glTexSubImage2D(Type,0,xoffset,yoffset,width,height,GL_RGB,GL_UNSIGNED_BYTE,data);
}

void Texture::Unset(uint8_t slot)
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(Type,0);

    active_slot = slot;
    current = Id;
}

void Texture::Free()
{

}

GLO_TYPE Texture::GetType()
{
    return GLO_TEXTURE;
}

