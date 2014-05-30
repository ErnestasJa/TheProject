#pragma once

#include "opengl/mesh.h"
#include "opengl/shader.h"
#include "math/rect2d.h"
#include "opengl/buffer_object.h"
#include "resources/resource_cache.h"
class sliced_gui_quad
{
private:
    float m_size;
    float m_margin;

    void create_verts()
    {
        float sx,sy;

        pos->data.resize(16);

        /// LL
        pos->data[0]=glm::vec3(-m_size,-m_size,0);

        pos->data[1]=glm::vec3(-m_size+m_margin,-m_size,0); /// X+
        pos->data[2]=glm::vec3(-m_size+m_margin,-m_size+m_margin,0); /// Y+ X+
        pos->data[3]=glm::vec3(-m_size,-m_size+m_margin,0); /// Y+
        /// ///////////////////////////////////////////////////////

        /// LR
        pos->data[4]=glm::vec3(m_size,-m_size,0);

        pos->data[5]=glm::vec3(m_size-m_margin,-m_size,0); /// X- Y
        pos->data[6]=glm::vec3(m_size-m_margin,-m_size+m_margin,0); /// X- Y+
        pos->data[7]=glm::vec3(m_size,-m_size+m_margin,0); /// X Y+
        /// ///////////////////////////////////////////////////////

        /// UR
        pos->data[8]=glm::vec3(m_size,m_size,0);

        pos->data[9]=glm::vec3(m_size-m_margin,m_size,0); /// X- Y
        pos->data[10]=glm::vec3(m_size-m_margin,m_size-m_margin,0); /// X- Y-
        pos->data[11]=glm::vec3(m_size,m_size-m_margin,0); /// X Y-
        /// ///////////////////////////////////////////////////////

        /// UL
        pos->data[12]=glm::vec3(-m_size,m_size,0);

        pos->data[13]=glm::vec3(-m_size+m_margin,m_size,0);
        pos->data[14]=glm::vec3(-m_size,m_size-m_margin,0);
        pos->data[15]=glm::vec3(-m_size+m_margin,m_size-m_margin,0);
        /// ///////////////////////////////////////////////////////
    }

    void create_inds()
    {

        indices->data=std::vector<uint32_t>({1,0,3,1,3,2,5,1,2,5,2,6,4,5,6,4,6,7,2,3,14,2,14,15,6,2,15,6,15,10,7,6,10,7,10,11,15,14,12,15,12,13,10,15,13,10,13,9,11,10,9,11,9,8});
    }
public:
    buffer_object<glm::vec3> *pos;
    buffer_object<glm::vec2> *tex_coords;
    index_buffer_object<uint32_t> *indices;

    std::shared_ptr<mesh> glmesh;

    static shader quad_shader_textured;

    sliced_gui_quad(float size=1.0f, float margin=0.125f)
    {
        this->m_size=size;
        this->m_margin=margin;

        pos=new buffer_object<glm::vec3>();
        tex_coords=new buffer_object<glm::vec2>();
        indices=new index_buffer_object<uint32_t>();
    }

    void create_tcoords(std::vector<glm::vec2> uvs)
    {

        tex_coords->data.resize(16);

        float tm = 1.f/256.f*8.f;

        glm::vec2 a=glm::vec2(helpers::limit(uvs[0].x,0.f,1.f),helpers::limit(uvs[0].y,0.f,1.f));
        glm::vec2 b=glm::vec2(helpers::limit(uvs[1].x,0.f,1.f),helpers::limit(uvs[1].y,0.f,1.f));
        glm::vec2 c=glm::vec2(helpers::limit(uvs[2].x,0.f,1.f),helpers::limit(uvs[2].y,0.f,1.f));
        glm::vec2 d=glm::vec2(helpers::limit(uvs[3].x,0.f,1.f),helpers::limit(uvs[3].y,0.f,1.f));

        tex_coords->data[0] = glm::vec2(a.x,a.y);
        tex_coords->data[1] = glm::vec2(a.x+tm,a.y);
        tex_coords->data[2] = glm::vec2(a.x+tm,a.y-tm);
        tex_coords->data[3] = glm::vec2(a.x,a.y-tm);

        tex_coords->data[4] = glm::vec2(b.x,b.y);
        tex_coords->data[5] = glm::vec2(b.x-tm,b.y);
        tex_coords->data[6] = glm::vec2(b.x-tm,b.y-tm);
        tex_coords->data[7] = glm::vec2(b.x,b.y-tm);

        tex_coords->data[8] = glm::vec2(c.x,c.y);
        tex_coords->data[9] = glm::vec2(c.x-tm,c.y);
        tex_coords->data[10] = glm::vec2(c.x-tm,c.y+tm);
        tex_coords->data[11] = glm::vec2(c.x,c.y+tm);

        tex_coords->data[12] = glm::vec2(d.x,d.y);
        tex_coords->data[13] = glm::vec2(d.x+tm,d.y);
        tex_coords->data[14] = glm::vec2(d.x,d.y+tm);
        tex_coords->data[15] = glm::vec2(d.x+tm,d.y+tm);
    }

    virtual ~sliced_gui_quad(){};

    virtual bool generate()
    {
        glmesh = share(new mesh());
        create_verts();
        std::vector<glm::vec2> defuv;
        defuv.push_back(glm::vec2(0,1));
        defuv.push_back(glm::vec2(1,1));
        defuv.push_back(glm::vec2(1,0));
        defuv.push_back(glm::vec2(0,0));
        create_tcoords(defuv);
        create_inds();

        glmesh->buffers.resize(3);
        glmesh->buffers[0]=pos;
        glmesh->buffers[1]=tex_coords;
        glmesh->buffers[2]=indices;


        glmesh->init();
        return true;
    }

    void draw()
    {
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,54,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    void set_tcoords(std::vector<glm::vec2> tcoords)
    {
        create_tcoords(tcoords);

        glBindBuffer(GL_ARRAY_BUFFER, glmesh->buffers[1]->id);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(tex_coords->data[0]), &tex_coords->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
