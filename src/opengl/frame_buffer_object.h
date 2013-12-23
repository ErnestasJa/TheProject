#pragma once

#include "opengl/texture.h"

enum FBO_TARGET
{
    FBO_READ = 0,
    FBO_WRITE,
    FBO_READ_AND_WRITE
};

struct frame_buffer_object
{
    uint32_t id;
    uint32_t target;

    uint8_t ebc;
    uint32_t enabled_buffer[8];

    std::shared_ptr<texture> color_binding[8];
    std::shared_ptr<texture> depth_buffer_binding;
    std::shared_ptr<texture> stencil_buffer_binding;
    std::shared_ptr<texture> depth_stencil_buffer_binding;

    frame_buffer_object()
    {
        id = 0;
        ebc = 0;
        target = GL_FRAMEBUFFER;
    }

    void enable_texture(uint32_t texture)
    {
        if(texture > 8)
            throw texture;

        texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < ebc; i++)
        {
            if(enabled_buffer[i]==texture)
                return;
        }

        enabled_buffer[ebc]=texture;
        ebc++;
    }

    void disable_texture(uint32_t texture)
    {
        if(texture > 8)
            throw texture;

        texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < ebc; i++)
        {
            if(enabled_buffer[i]==texture)
            {
                ebc--;
                for(uint8_t j = i; j < ebc; j++)
                    enabled_buffer[j]=enabled_buffer[j+1];
            }
        }
    }

    bool generate()
    {
        glGenFramebuffers(1,&id);
        return true;
    }

    void set(FBO_TARGET target)
    {
        if(target == FBO_READ)
            this->target = GL_READ_FRAMEBUFFER;
        else if(target == FBO_WRITE)
            this->target = GL_DRAW_FRAMEBUFFER;
        else if(target == FBO_READ_AND_WRITE)
            this->target = GL_FRAMEBUFFER;
        else
            throw target;

        glBindFramebuffer(this->target, id);
        glDrawBuffers(ebc,enabled_buffer);
    }

    void unset()
    {
        glBindFramebuffer(target,0);
    }

    void attach_depth_texture(std::shared_ptr<texture> tex, uint32_t level=0)
    {
        glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,tex->type,tex->obj,level);
        depth_buffer_binding=tex;
    }

    void attach_stencil_texture(std::shared_ptr<texture> tex, uint32_t level=0)
    {
        glFramebufferTexture2D(target,GL_STENCIL_ATTACHMENT,tex->type,tex->obj,level);
        stencil_buffer_binding=tex;
    }

    void attach_depth_stencil_texture(std::shared_ptr<texture> tex, uint32_t level=0)
    {
        glFramebufferTexture2D(target,GL_DEPTH_STENCIL_ATTACHMENT,tex->type,tex->obj,level);
        depth_stencil_buffer_binding=tex;
    }

    void attach_texture(uint32_t attachment_point, std::shared_ptr<texture> tex, uint32_t level=0)
    {
        if(attachment_point < 8)
        {
            color_binding[attachment_point]=tex;
            glFramebufferTexture2D(target, attachment_point+GL_COLOR_ATTACHMENT0, tex->type, tex->obj, level);
        }
    }

    void detach_texture(uint32_t attachment_point)
    {
        if(attachment_point < 8)
        {
            color_binding[attachment_point]=nullptr;
            glFramebufferTexture2D(target,attachment_point+GL_COLOR_ATTACHMENT0,0,0,0);
        }
    }

    void detach_depth_texture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,0,0,0);
        depth_buffer_binding=nullptr;
    }

    void detach_stencil_texture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,0,0,0);
        stencil_buffer_binding=nullptr;
    }

    void detach_depth_stencil_texture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_STENCIL_ATTACHMENT,0,0,0);
        depth_stencil_buffer_binding=nullptr;
    }

    bool is_complete()
    {
        return glCheckFramebufferStatus(target)==GL_FRAMEBUFFER_COMPLETE;
    }

    uint32_t get_status()
    {
        return glCheckFramebufferStatus(target);
    }
};
