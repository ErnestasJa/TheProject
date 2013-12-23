#pragma once

#include "opengl/texture.h"

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

    void enable(uint32_t buffer)
    {
        for(uint8_t i = 0; i < ebc; i++)
        {
            if(enabled_buffer[i]==buffer)
                return;
        }

        enabled_buffer[ebc]=buffer;
        ebc++;
    }

    void disable(uint32_t buffer)
    {
        for(uint8_t i = 0; i < ebc; i++)
        {
            if(enabled_buffer[i]==buffer)
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

    void set(uint32_t target)
    {
        this->target = target;
        glBindFramebuffer(target, id);

        glDrawBuffers(ebc,enabled_buffer);
    }

    void unset()
    {
        glBindFramebuffer(target,0);
    }

    void attach(uint32_t attachment_point, std::shared_ptr<texture> tex, uint32_t level=0)
    {
        glFramebufferTexture2D(target,attachment_point,tex->type,tex->obj,0);

        if(attachment_point>=GL_COLOR_ATTACHMENT0 && attachment_point<GL_COLOR_ATTACHMENT0+8)
            color_binding[attachment_point-GL_COLOR_ATTACHMENT0]=tex;
        else if(attachment_point==GL_DEPTH_ATTACHMENT)
            depth_buffer_binding=tex;
        else if(attachment_point==GL_STENCIL_ATTACHMENT)
            stencil_buffer_binding=tex;
        else if(attachment_point==GL_DEPTH_STENCIL_ATTACHMENT)
            depth_stencil_buffer_binding=tex;

    }

    void detach(uint32_t attachment_point)
    {
        glFramebufferTexture2D(target,attachment_point,0,0,0);

        if(attachment_point>=GL_COLOR_ATTACHMENT0 && attachment_point<GL_COLOR_ATTACHMENT0+8)
            color_binding[attachment_point-GL_COLOR_ATTACHMENT0]=NULL;
        else if(attachment_point==GL_DEPTH_ATTACHMENT)
            depth_buffer_binding=NULL;
        else if(attachment_point==GL_STENCIL_ATTACHMENT)
            stencil_buffer_binding=NULL;
        else if(attachment_point==GL_DEPTH_STENCIL_ATTACHMENT)
            depth_stencil_buffer_binding=NULL;
    }

    uint32_t is_complete()
    {
        return glCheckFramebufferStatus(target);
    }
};
