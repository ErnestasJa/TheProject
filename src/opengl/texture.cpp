#include "precomp.h"
#include "resources/image.h"
#include "texture.h"

uint32_t texture::active_slot = 0;
uint32_t texture::current = 0;


texture::texture()
{
    obj = 0;
    type = 0;
}

texture::~texture()
{
    free();
}

void texture::init(std::shared_ptr<image> img)
{
    type = GL_TEXTURE_2D;

    glGenTextures(1, &obj);
    glBindTexture(type,obj);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    /// either use texture with or without alpha channel
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

    glTexImage2D(type,0,GL_RGBA,img->width,img->height,0,img->num_channels==4?GL_RGBA:GL_RGB,GL_UNSIGNED_BYTE,img->data);

    glGenerateMipmap(type);

    glBindTexture(type,current);
}

/**
 * @brief
 * @param clamp
 * bit0 = s to edge, bit1 = t to edge
 * @param filter
 * bit0 = linear, bit1 = mipmap
 */
void texture::init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h)
{
    type = target;

    glGenTextures(1, &obj);
    glBindTexture(type,obj);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /// either use texture with or without alpha channel
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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }

    glTexImage2D(type,0,internal_format,w,h,0,image_format,image_format==GL_DEPTH_COMPONENT?GL_UNSIGNED_INT:GL_UNSIGNED_BYTE,data);

    //glinitMipmap(type);

    glBindTexture(type,current);
}

void texture::set_filters(FILTER_MIN fmin, FILTER_MAG fmag)
{
    glBindTexture(type,obj);

    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, fmin == FILTER_MIN_LINEAR ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, fmag == FILTER_MAG_LINEAR_MIPMAP ? GL_LINEAR_MIPMAP_LINEAR : (fmag==FILTER_MAG_NEAREST_MIPMAP ? GL_NEAREST_MIPMAP_NEAREST : (fmag==FILTER_MAG_LINEAR ? GL_LINEAR : GL_NEAREST)));

    glBindTexture(type,current);
}

void texture::set_clamp(texture::CLAMP x, texture::CLAMP y)
{
    glBindTexture(type,obj);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, x == CLAMP_EDGE ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, y == CLAMP_EDGE ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    glBindTexture(type,current);
}

void texture::set(uint8_t slot)
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(type,obj);

    active_slot = slot;
    current = obj;
}

void texture::free()
{
    glDeleteTextures(1,&obj);
}
