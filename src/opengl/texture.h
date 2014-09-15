#pragma once

#include "GLObject.h"

struct image;
struct Texture: public GLObject
{
    enum FILTER_MIN
    {
        FILTER_MIN_NEAREST = 0,
        FILTER_MIN_LINEAR,
        FILTER_MIN_LINEAR_MIPMAP,
        FILTER_MIN_NEAREST_MIPMAP
    };

    enum FILTER_MAG
    {
        FILTER_MAG_NEAREST = 0,
        FILTER_MAG_LINEAR,
    };

    enum CLAMP
    {
        CLAMP_REPEAT = 0,
        CLAMP_EDGE,
        CLAMP_BORDER
    };

    std::string name;
    uint32_t Type;

    static uint32_t current, active_slot;

    Texture();
    ~Texture();

    void Init(std::shared_ptr<image> img);
    void Init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h);

    void SetFilters(Texture::FILTER_MIN fmin, Texture::FILTER_MAG fmag);
    void SetClampMode(Texture::CLAMP x, Texture::CLAMP y);
    void SetSubImage2D(const uint8_t * data,uint32_t xoffset,uint32_t yoffset,uint32_t width,uint32_t height);
    void SetBorderColor(const glm::vec4 & color);
    void InitMipmap(uint32_t base, uint32_t max);
    void InitMipmap();
    void UpdateMipmaps();
    void Set(uint8_t slot);
    void Unset(uint8_t slot);
    void Free();
    virtual GLO_TYPE GetType();
};

typedef std::shared_ptr<Texture> TexturePtr;
