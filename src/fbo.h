#pragma once

#include "texture.h"
#include "precomp.h"

struct fbo
{
    static uint32_t current, current_target;

    uint32_t id;
    uint32_t target;

    fbo()
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
        if(tex)
            glFramebufferTexture2D(target,attachment_point,tex->type,tex->obj,0);
    }

    void detach(uint32_t attachment_point, uint32_t tex_target)
    {
        glFramebufferTexture2D(target,attachment_point,tex_target,0,0);
    }

    bool is_complete()
    {
        return glCheckFramebufferStatus(target)==GL_FRAMEBUFFER_COMPLETE;
    }
};
