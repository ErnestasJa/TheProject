#pragma once

struct image;
struct texture
{
    enum FILTER_MIN
    {
        FILTER_MIN_NEAREST = 0,
        FILTER_MIN_LINEAR,
    };

    enum FILTER_MAG
    {
        FILTER_MAG_NEAREST = 0,
        FILTER_MAG_LINEAR,
        FILTER_MAG_LINEAR_MIPMAP,
        FILTER_MAG_NEAREST_MIPMAP
    };

    enum CLAMP
    {
        CLAMP_REPEAT = 0,
        CLAMP_EDGE,
    };

    std::string name;
    uint32_t obj;
    uint32_t type;

    static uint32_t current, active_slot;

    texture();
    ~texture();

    void generate(std::shared_ptr<image> img);
    void generate(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h);

    void set_filters(texture::FILTER_MIN fmin, texture::FILTER_MAG fmag);
    void set_clamp(texture::CLAMP x, texture::CLAMP y);
    void set(uint8_t slot);
    void free();


};
