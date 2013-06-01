#pragma once

#include "texture.h"
#include "precomp.h"

struct frame_buffer_object
{
    static uint32_t current, current_target;

    uint32_t id;
    uint32_t target;

    std::shared_ptr<texture> color_binding[8];
    std::shared_ptr<texture> depth_buffer_binding;
    std::shared_ptr<texture> stencil_buffer_binding;
    std::shared_ptr<texture> depth_stencil_buffer_binding;

    frame_buffer_object()
    {
        id = 0;
        target = GL_FRAMEBUFFER;
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
