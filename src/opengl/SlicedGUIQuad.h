#pragma once

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "utility/Rect2d.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"
class SlicedGUIQuad
{
private:
    float m_size;
    float m_margin;

    void CreateVerts()
    {
        //float sx,sy;

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

    void CreateInds()
    {

        indices->data=std::vector<uint32_t>({1,3,0,1,2,3,5,2,1,5,6,2,4,6,5,4,7,6,2,14,3,2,15,14,6,15,2,6,10,15,7,10,6,7,11,10,15,12,14,15,13,12,10,13,15,10,9,13,11,9,10,11,8,9});
    }
public:
    BufferObject<glm::vec3> *pos;
    BufferObject<glm::vec2> *tex_coords;
    IndexBufferObject<uint32_t> *indices;

    std::shared_ptr<Mesh> glmesh;

    static Shader quad_shader_textured;

    SlicedGUIQuad(float size=1.0f, float margin=0.125f)
    {
        this->m_size=size;
        this->m_margin=margin;

        pos=new BufferObject<glm::vec3>();
        tex_coords=new BufferObject<glm::vec2>();
        indices=new IndexBufferObject<uint32_t>();
    }

    void CreateTCoords(std::vector<glm::vec2> uvs)
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

    virtual ~SlicedGUIQuad(){};

    virtual bool Init()
    {
        glmesh = share(new Mesh());
        CreateVerts();
        std::vector<glm::vec2> defuv;
        defuv.push_back(glm::vec2(0,1));
        defuv.push_back(glm::vec2(1,1));
        defuv.push_back(glm::vec2(1,0));
        defuv.push_back(glm::vec2(0,0));
        CreateTCoords(defuv);
        CreateInds();

        glmesh->buffers.resize(3);
        glmesh->buffers[0]=pos;
        glmesh->buffers[1]=tex_coords;
        glmesh->buffers[2]=indices;


        glmesh->Init();
        return true;
    }

    void Render()
    {
        //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        //glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,54,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
        //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    void SetTCoords(std::vector<glm::vec2> tcoords)
    {
        CreateTCoords(tcoords);

        glBindBuffer(GL_ARRAY_BUFFER, glmesh->buffers[1]->Id);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(tex_coords->data[0]), &tex_coords->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
