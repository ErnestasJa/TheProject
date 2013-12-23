#pragma once

struct image
{
    std::string name;
    glm::vec2 size;
    uint32_t bpp; /// >bits< per pixel
};

struct texture
{
    std::string name;
    uint32_t obj;
    uint32_t type;

    static uint32_t current, active_slot;

    texture()
    {
        obj = 0;
        type = 0;
    }

    ~texture()
    {
        free();
    }

    /**
     * @brief
     * @param clamp
     * bit0 = s to edge, bit1 = t to edge
     * @param filter
     * bit0 = linear, bit1 = mipmap
     */
    void generate(const uint8_t * data, uint32_t target, uint32_t image_format, uint32_t internal_format, int32_t w, int32_t h, uint8_t filter=bit<0>(), uint8_t clamp=bit<0>()|bit<1>())
    {
        type = target;

        glGenTextures(1, &obj);
        glBindTexture(type,obj);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, tbit(clamp,BIT0) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, tbit(clamp,BIT1) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, tbit(filter,BIT1) ? (tbit(filter,BIT0)?GL_LINEAR_MIPMAP_LINEAR:GL_NEAREST_MIPMAP_NEAREST) : (tbit(filter,BIT0)?GL_LINEAR:GL_NEAREST) );

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

        if(tbit(filter,BIT1)) glGenerateMipmap(type);

        glBindTexture(type,current);
    }

    void set_filters(uint8_t filter=bit<0>())
    {
        glBindTexture(type,obj);

        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, tbit(filter,BIT1) ? GL_LINEAR_MIPMAP_LINEAR : (tbit(filter,BIT0) ? GL_LINEAR : GL_NEAREST));

        glBindTexture(type,current);
    }

    void set_clamp(uint8_t clamp=bit<0>()|bit<1>())
    {
        glBindTexture(type,obj);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, tbit(clamp,BIT0) ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, tbit(clamp,BIT1) ? GL_CLAMP_TO_EDGE : GL_REPEAT);

        glBindTexture(type,current);
    }

    void set(uint8_t slot)
    {
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(type,obj);

        active_slot = slot;
        current = obj;
    }

    void free()
    {
        glDeleteTextures(1,&obj);
    }

};
