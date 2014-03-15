#pragma once

#include "texture.h"
#include "render_buffer_object.h"
#include "gl_object.h"

enum FBO_TARGET
{
    FBO_READ = 0,
    FBO_WRITE,
    FBO_READ_AND_WRITE
};

struct frame_buffer_object: public gl_object
{
    uint32_t target;

    uint8_t enabled_buffer_count;
    uint32_t enabled_buffer[8];

    gl_object_ptr color_binding[8];
    gl_object_ptr depth_buffer_binding;
    gl_object_ptr stencil_buffer_binding;
    gl_object_ptr depth_stencil_buffer_binding;

    frame_buffer_object()
    {
        id = 0;
        enabled_buffer_count = 0;
        target = GL_FRAMEBUFFER;
    }

    ~frame_buffer_object()
    {
        if(id)
        glDeleteFramebuffers(1,&id);
    }

    void enable_buffer(uint32_t texture)
    {
        if(texture > 8)
            throw texture;

        texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < enabled_buffer_count; i++)
        {
            if(enabled_buffer[i]==texture)
                return;
        }

        enabled_buffer[enabled_buffer_count]=texture;
        enabled_buffer_count++;

        glDrawBuffers(enabled_buffer_count,enabled_buffer);
    }

    void disable_buffer(uint32_t texture)
    {
        if(texture > 8)
            throw texture;

        texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < enabled_buffer_count; i++)
        {
            if(enabled_buffer[i]==texture)
            {
                enabled_buffer_count--;
                for(uint8_t j = i; j < enabled_buffer_count; j++)
                    enabled_buffer[j]=enabled_buffer[j+1];
            }
        }

        glDrawBuffers(enabled_buffer_count,enabled_buffer);
    }

    void disable_all_buffers()
    {
        for(uint8_t j = 0; j < 8; j++)
            enabled_buffer[j]=GL_NONE;

        glDrawBuffers(8,enabled_buffer);
    }

    void set_buffer(uint32_t buffer)
    {
        if(buffer > 8)
            throw buffer;

        buffer += GL_COLOR_ATTACHMENT0;

        glDrawBuffer(buffer);
    }

    bool init()
    {
        glGenFramebuffers(1,&id);
        return true;
    }

    void set()
    {
        glBindFramebuffer(target, id);
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
    }

    void unset()
    {
        glBindFramebuffer(target,0);
        glDrawBuffer(GL_BACK_LEFT);
    }

    void attach_depth_texture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->get_type()==GLO_TEXTURE)
        {
            texture_ptr tex = std::static_pointer_cast<texture>(obj);
            glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,tex->type,tex->id,level);
        }
        else if(obj->get_type()==GLO_RENDERBUFFER)
        {
            render_buffer_object_ptr rbo = std::static_pointer_cast<render_buffer_object>(obj);
            glFramebufferRenderbuffer(target,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->id);
        }

        depth_buffer_binding=obj;
    }

    void attach_stencil_texture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->get_type()==GLO_TEXTURE)
        {
            texture_ptr tex = std::static_pointer_cast<texture>(obj);
            glFramebufferTexture2D(target,GL_STENCIL_ATTACHMENT,tex->type,tex->id,level);
        }
        else if(obj->get_type()==GLO_RENDERBUFFER)
        {
            render_buffer_object_ptr rbo = std::static_pointer_cast<render_buffer_object>(obj);
            glFramebufferRenderbuffer(target,GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->id);
        }

        stencil_buffer_binding=obj;
    }

    void attach_depth_stencil_texture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->get_type()==GLO_TEXTURE)
        {
            texture_ptr tex = std::static_pointer_cast<texture>(obj);
            glFramebufferTexture2D(target,GL_DEPTH_STENCIL_ATTACHMENT,tex->type,tex->id,level);
        }
        else if(obj->get_type()==GLO_RENDERBUFFER)
        {
            render_buffer_object_ptr rbo = std::static_pointer_cast<render_buffer_object>(obj);
            glFramebufferRenderbuffer(target,GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->id);
        }

        depth_stencil_buffer_binding=obj;
    }

    void attach_texture(uint32_t attachment_point, gl_object_ptr obj, uint32_t level=0)
    {
        if(attachment_point < 8)
        {
            if(obj->get_type()==GLO_TEXTURE)
            {
                texture_ptr tex = std::static_pointer_cast<texture>(obj);
                glFramebufferTexture2D(target, attachment_point+GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id, level);
            }
            else if(obj->get_type()==GLO_RENDERBUFFER)
            {
                render_buffer_object_ptr rbo = std::static_pointer_cast<render_buffer_object>(obj);
                glFramebufferRenderbuffer(target,attachment_point+GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo->id);
            }

            color_binding[attachment_point]=obj;
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

    GLO_TYPE get_type()
    {
        return GLO_FRAMEBUFFER;
    }

};

typedef std::shared_ptr<frame_buffer_object> frame_buffer_object_ptr;
