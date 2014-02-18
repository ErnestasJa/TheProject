#pragma once

#include "resources/mesh.h"
#include "opengl/shader.h"
#include "math/rect2d.h"
class sliced_gui_quad
{
private:
    float m_size;
    float m_margin;
    void create_verts()
    {
        pos[0]=glm::vec3(-m_size,-m_size,0);
        pos[1]=glm::vec3(-m_size+m_margin,-m_size,0);
        pos[4]=glm::vec3(-m_size,-m_size+m_margin,0);
        pos[5]=glm::vec3(-m_size+m_margin,-m_size+m_margin,0);

        pos[2]=glm::vec3(m_size-m_margin,-m_size,0);
        pos[3]=glm::vec3(m_size,-m_size,0);
        pos[6]=glm::vec3(m_size-m_margin,-m_size+m_margin,0);
        pos[7]=glm::vec3(m_size,-m_size+m_margin,0);

        pos[8]=glm::vec3(-m_size,m_size-m_margin,0);
        pos[9]=glm::vec3(-m_size+m_margin,m_size-m_margin,0);
        pos[12]=glm::vec3(-m_size,m_size,0);
        pos[13]=glm::vec3(-m_size+m_margin,m_size,0);

        pos[10]=glm::vec3(m_size-m_margin,m_size-m_margin,0);
        pos[11]=glm::vec3(m_size,m_size-m_margin,0);
        pos[14]=glm::vec3(m_size-m_margin,m_size,0);
        pos[15]=glm::vec3(m_size,m_size,0);
    }

    void create_inds()
    {
        indices=new uint32_t[54]{0,1,5,5,4,0,1,2,6,6,5,1,2,3,7,7,6,2,4,5,9,9,8,4,5,6,10,10,9,5,6,7,11,11,10,6,8,9,13,13,12,8,9,10,14,14,13,9,10,11,15,15,14,10};
    }
public:
    glm::vec3 pos[16];
    glm::vec2 tex_coords[16];
    uint32_t *indices;

    mesh glmesh;

    static shader quad_shader_textured;

    sliced_gui_quad(float size=1.0f, float margin=0.1f)
    {
        this->m_size=size;
        this->m_margin=margin;
    }

    void create_tcoords(glm::vec2* uvs)
    {
        float tm = 0.00125;

        glm::vec2 a=uvs[0];
        glm::vec2 b=uvs[1];
        glm::vec2 c=uvs[2];
        glm::vec2 d=uvs[3];

        tex_coords[0] = glm::vec2(a.x,a.y);
        tex_coords[1] = glm::vec2(a.x+tm,a.y);
        tex_coords[4] = glm::vec2(a.x,a.y-tm);
        tex_coords[5] = glm::vec2(a.x+tm,a.y-tm);

        tex_coords[2] = glm::vec2(b.x-tm,b.y);
        tex_coords[3] = glm::vec2(b.x,b.y);
        tex_coords[6] = glm::vec2(b.x-tm,b.y-tm);
        tex_coords[7] = glm::vec2(b.x,b.y-tm);

        tex_coords[8] = glm::vec2(c.x,c.y+tm);
        tex_coords[9] = glm::vec2(c.x+tm,c.y+tm);
        tex_coords[12] = glm::vec2(c.x,c.y);
        tex_coords[13] = glm::vec2(c.x+tm,c.y);

        tex_coords[10] = glm::vec2(d.x-tm,d.y+tm);
        tex_coords[11] = glm::vec2(d.x,d.y+tm);
        tex_coords[14] = glm::vec2(d.x-tm,d.y);
        tex_coords[15] = glm::vec2(d.x,d.y);
    }

    virtual ~sliced_gui_quad(){};

    virtual bool generate()
    {
        create_verts();
        glm::vec2*defuv=new glm::vec2[4]{glm::vec2(0,1),glm::vec2(1,1),glm::vec2(0,0),glm::vec2(1,0)};
        create_tcoords(defuv);
        create_inds();

        glmesh.buffers.resize(3);

        glGenVertexArrays(1,&glmesh.vao);

        glBindVertexArray(glmesh.vao);

        glGenBuffers(1, &glmesh.buffers[0]);
        glBindBuffer(GL_ARRAY_BUFFER, glmesh.buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(pos[0]), pos, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &glmesh.buffers[1]);
        glBindBuffer(GL_ARRAY_BUFFER, glmesh.buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(tex_coords[0]), &tex_coords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1,&glmesh.buffers[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glmesh.buffers[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,54*sizeof(uint32_t),&indices[0],GL_STATIC_DRAW);

        glBindVertexArray(0);

        return true;
    }

    void draw()
    {
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(glmesh.vao);
        glDrawElements(GL_TRIANGLES,54,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    void set_tcoords(glm::vec2 *tcoords)
    {
        create_tcoords(tcoords);

        glBindBuffer(GL_ARRAY_BUFFER, glmesh.buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(tex_coords[0]), &tex_coords[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
