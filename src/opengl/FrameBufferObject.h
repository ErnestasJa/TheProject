#pragma once

#include "Texture.h"
#include "RenderBufferObject.h"
#include "GLObject.h"

enum FBO_TARGET
{
    FBO_READ = 0,
    FBO_WRITE,
    FBO_READ_AND_WRITE
};

struct FrameBufferObject: public GLObject
{
    uint32_t target;

    uint8_t enabled_buffer_count;
    uint32_t enabled_buffer[8];

    gl_object_ptr color_binding[8];
    gl_object_ptr depth_buffer_binding;
    gl_object_ptr stencil_buffer_binding;
    gl_object_ptr depth_stencil_buffer_binding;

    FrameBufferObject()
    {
        Id = 0;
        enabled_buffer_count = 0;
        target = GL_FRAMEBUFFER;
    }

    ~FrameBufferObject()
    {
        if(Id)
        glDeleteFramebuffers(1,&Id);
    }

    void EnableBuffer(uint32_t Texture)
    {
        if(Texture > 8)
            throw Texture;

        Texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < enabled_buffer_count; i++)
        {
            if(enabled_buffer[i]==Texture)
                return;
        }

        enabled_buffer[enabled_buffer_count]=Texture;
        enabled_buffer_count++;

        glDrawBuffers(enabled_buffer_count,enabled_buffer);
    }

    void DisableBuffer(uint32_t Texture)
    {
        if(Texture > 8)
            throw Texture;

        Texture += GL_COLOR_ATTACHMENT0;

        for(uint8_t i = 0; i < enabled_buffer_count; i++)
        {
            if(enabled_buffer[i]==Texture)
            {
                enabled_buffer_count--;
                for(uint8_t j = i; j < enabled_buffer_count; j++)
                    enabled_buffer[j]=enabled_buffer[j+1];
            }
        }

        glDrawBuffers(enabled_buffer_count,enabled_buffer);
    }

    void EnableSingleBuffer(uint32_t Texture)
    {
        if(Texture > 8)
            throw Texture;

        Texture += GL_COLOR_ATTACHMENT0;
        enabled_buffer_count = 1;
        enabled_buffer[0]=Texture;

        glDrawBuffers(1,enabled_buffer);
    }

    bool Init()
    {
        glGenFramebuffers(1,&Id);
        return true;
    }

    void Set()
    {
        glBindFramebuffer(target, Id);
    }

    void Set(FBO_TARGET target)
    {
        if(target == FBO_READ)
            this->target = GL_READ_FRAMEBUFFER;
        else if(target == FBO_WRITE)
            this->target = GL_DRAW_FRAMEBUFFER;
        else if(target == FBO_READ_AND_WRITE)
            this->target = GL_FRAMEBUFFER;
        else
            throw target;

        glBindFramebuffer(this->target, Id);
    }

    void Unset()
    {
        glBindFramebuffer(target,0);
    }

    void AttachDepthTexture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->GetType()==GLO_TEXTURE)
        {
            TexturePtr tex = std::static_pointer_cast<Texture>(obj);
            glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,tex->Type,tex->Id,level);
        }
        else if(obj->GetType()==GLO_RENDERBUFFER)
        {
            render_BufferObject_ptr rbo = std::static_pointer_cast<RenderBufferObject>(obj);
            glFramebufferRenderbuffer(target,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo->Id);
        }

        depth_buffer_binding=obj;
    }

    void AttachStencilTexture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->GetType()==GLO_TEXTURE)
        {
            TexturePtr tex = std::static_pointer_cast<Texture>(obj);
            glFramebufferTexture2D(target,GL_STENCIL_ATTACHMENT,tex->Type,tex->Id,level);
        }
        else if(obj->GetType()==GLO_RENDERBUFFER)
        {
            render_BufferObject_ptr rbo = std::static_pointer_cast<RenderBufferObject>(obj);
            glFramebufferRenderbuffer(target,GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->Id);
        }

        stencil_buffer_binding=obj;
    }

    void AttachDepthStencilTexture(gl_object_ptr obj, uint32_t level=0)
    {
        if(obj->GetType()==GLO_TEXTURE)
        {
            TexturePtr tex = std::static_pointer_cast<Texture>(obj);
            glFramebufferTexture2D(target,GL_DEPTH_STENCIL_ATTACHMENT,tex->Type,tex->Id,level);
        }
        else if(obj->GetType()==GLO_RENDERBUFFER)
        {
            render_BufferObject_ptr rbo = std::static_pointer_cast<RenderBufferObject>(obj);
            glFramebufferRenderbuffer(target,GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->Id);
        }

        depth_stencil_buffer_binding=obj;
    }

    void AttachTexture(uint32_t attachment_point, gl_object_ptr obj, uint32_t level=0)
    {
        if(attachment_point < 8)
        {
            if(obj->GetType()==GLO_TEXTURE)
            {
                TexturePtr tex = std::static_pointer_cast<Texture>(obj);
                glFramebufferTexture2D(target, attachment_point+GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->Id, level);
            }
            else if(obj->GetType()==GLO_RENDERBUFFER)
            {
                render_BufferObject_ptr rbo = std::static_pointer_cast<RenderBufferObject>(obj);
                glFramebufferRenderbuffer(target,attachment_point+GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo->Id);
            }

            color_binding[attachment_point]=obj;
        }
    }

    void DetachTexture(uint32_t attachment_point)
    {
        if(attachment_point < 8)
        {
            color_binding[attachment_point]=nullptr;
            glFramebufferTexture2D(target,attachment_point+GL_COLOR_ATTACHMENT0,0,0,0);
        }
    }

    void DetachDepthTexture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,0,0,0);
        depth_buffer_binding=nullptr;
    }

    void DetachStencilTexture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_ATTACHMENT,0,0,0);
        stencil_buffer_binding=nullptr;
    }

    void DetachDepthStencilTexture()
    {
        glFramebufferTexture2D(target,GL_DEPTH_STENCIL_ATTACHMENT,0,0,0);
        depth_stencil_buffer_binding=nullptr;
    }

    bool IsComplete()
    {
        return glCheckFramebufferStatus(target)==GL_FRAMEBUFFER_COMPLETE;
    }

    uint32_t GetStatus()
    {
        return glCheckFramebufferStatus(target);
    }

    GLO_TYPE GetType()
    {
        return GLO_FRAMEBUFFER;
    }

};

typedef std::shared_ptr<FrameBufferObject> frame_BufferObject_ptr;
