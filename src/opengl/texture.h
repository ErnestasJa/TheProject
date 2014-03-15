#pragma once

#include "gl_object.h"

struct image;
struct texture: public gl_object
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
    uint32_t type;

    static uint32_t current, active_slot;

    texture();
    ~texture();

    void init(std::shared_ptr<image> img);
    void init(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h);

    void set_filters(texture::FILTER_MIN fmin, texture::FILTER_MAG fmag);
    void set_clamp(texture::CLAMP x, texture::CLAMP y);
    void set_border_color(const glm::vec4 & color);
    void init_mipmap(uint32_t base, uint32_t max);
    void init_mipmap();
    void set(uint8_t slot);
    void free();
    virtual GLO_TYPE get_type();
};

typedef std::shared_ptr<texture> texture_ptr;
