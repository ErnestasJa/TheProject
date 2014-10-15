#pragma once

#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "utility/Rect2d.h"
#include "opengl/BufferObject.h"
#include "resources/ResourceCache.h"
class SlicedGUIQuad
{
private:
    glm::vec2 m_size;
    glm::vec2 ratio;
    float m_margin;

    void CreateVerts()
    {
        pos->data.resize(16);

        /// LL
        pos->data[0]=glm::vec3(-m_size.x,-m_size.y,0);

        pos->data[1]=glm::vec3(-m_size.x+m_margin*ratio.x,-m_size.y,0); /// X+
        pos->data[2]=glm::vec3(-m_size.x+m_margin*ratio.x,-m_size.y+m_margin*ratio.y,0); /// Y+ X+
        pos->data[3]=glm::vec3(-m_size.x,-m_size.y+m_margin*ratio.y,0); /// Y+
        /// ///////////////////////////////////////////////////////

        /// LR
        pos->data[4]=glm::vec3(m_size.x,-m_size.y,0);

        pos->data[5]=glm::vec3(m_size.x-m_margin*ratio.x,-m_size.y,0); /// X- Y
        pos->data[6]=glm::vec3(m_size.x-m_margin*ratio.x,-m_size.y+m_margin*ratio.y,0); /// X- Y+
        pos->data[7]=glm::vec3(m_size.x,-m_size.y+m_margin*ratio.y,0); /// X Y+
        /// ///////////////////////////////////////////////////////

        /// UR
        pos->data[8]=glm::vec3(m_size.x,m_size.y,0);

        pos->data[9]=glm::vec3(m_size.x-m_margin*ratio.x,m_size.y,0); /// X- Y
        pos->data[10]=glm::vec3(m_size.x-m_margin*ratio.x,m_size.y-m_margin*ratio.y,0); /// X- Y-
        pos->data[11]=glm::vec3(m_size.x,m_size.y-m_margin*ratio.y,0); /// X Y-
        /// ///////////////////////////////////////////////////////

        /// UL
        pos->data[12]=glm::vec3(-m_size.x,m_size.y,0);

        pos->data[13]=glm::vec3(-m_size.x+m_margin*ratio.x,m_size.y,0);
        pos->data[14]=glm::vec3(-m_size.x,m_size.y-m_margin*ratio.y,0);
        pos->data[15]=glm::vec3(-m_size.x+m_margin*ratio.x,m_size.y-m_margin*ratio.y,0);
        /// ///////////////////////////////////////////////////////
    }

    void CreateInds()
    {
        const static uint32_t ind_vec[]{1,3,0,1,2,3,5,2,1,5,6,2,4,6,5,4,7,6,2,14,3,2,15,14,6,15,2,6,10,15,7,10,6,7,11,10,15,12,14,15,13,12,10,13,15,10,9,13,11,9,10,11,8,9};
        indices->data.insert(indices->data.begin(),ind_vec,ind_vec+54);
    }
public:
    BufferObject<glm::vec3> *pos;
    BufferObject<glm::vec2> *tex_coords;
    IndexBufferObject<uint32_t> *indices;

    std::shared_ptr<Mesh> glmesh;

    static Shader quad_shader_textured;

    SlicedGUIQuad(glm::vec2 size=glm::vec2(1), float margin=1.f)
    {
        float minRatio=std::min(size.x,size.y)/std::max(size.x,size.y);
        if(size.x>size.y)
        {
            this->m_size.x=1.f;
            this->m_size.y=minRatio;
        }
        else if(size.x<size.y)
        {
            this->m_size.x=minRatio;
            this->m_size.y=1.f;
        }
        else
        {
            this->m_size.x=1.f;
            this->m_size.y=1.f;
        }

        this->m_margin=margin;

        pos=new BufferObject<glm::vec3>();
        tex_coords=new BufferObject<glm::vec2>();
        indices=new IndexBufferObject<uint32_t>();
    }

    void CreateTCoords(vector<glm::vec2> uvs)
    {

        tex_coords->data.resize(16);

        glm::vec2 tm(1);
        tm.x = 1.f/256.f*m_margin;
        tm.y = 1.f/256.f*m_margin;

        glm::vec2 a=glm::vec2(helpers::limit(uvs[0].x,0.f,1.f),helpers::limit(uvs[0].y,0.f,1.f));
        glm::vec2 b=glm::vec2(helpers::limit(uvs[1].x,0.f,1.f),helpers::limit(uvs[1].y,0.f,1.f));
        glm::vec2 c=glm::vec2(helpers::limit(uvs[2].x,0.f,1.f),helpers::limit(uvs[2].y,0.f,1.f));
        glm::vec2 d=glm::vec2(helpers::limit(uvs[3].x,0.f,1.f),helpers::limit(uvs[3].y,0.f,1.f));

        tex_coords->data[0] = glm::vec2(a.x,a.y);
        tex_coords->data[1] = glm::vec2(a.x+tm.x,a.y);
        tex_coords->data[2] = glm::vec2(a.x+tm.x,a.y-tm.y);
        tex_coords->data[3] = glm::vec2(a.x,a.y-tm.y);

        tex_coords->data[4] = glm::vec2(b.x,b.y);
        tex_coords->data[5] = glm::vec2(b.x-tm.x,b.y);
        tex_coords->data[6] = glm::vec2(b.x-tm.x,b.y-tm.y);
        tex_coords->data[7] = glm::vec2(b.x,b.y-tm.y);

        tex_coords->data[8] = glm::vec2(c.x,c.y);
        tex_coords->data[9] = glm::vec2(c.x-tm.x,c.y);
        tex_coords->data[10] = glm::vec2(c.x-tm.x,c.y+tm.y);
        tex_coords->data[11] = glm::vec2(c.x,c.y+tm.y);

        tex_coords->data[12] = glm::vec2(d.x,d.y);
        tex_coords->data[13] = glm::vec2(d.x+tm.x,d.y);
        tex_coords->data[14] = glm::vec2(d.x,d.y+tm.y);
        tex_coords->data[15] = glm::vec2(d.x+tm.x,d.y+tm.y);
    }

    virtual ~SlicedGUIQuad(){};

    virtual bool Init()
    {
        glmesh = share(new Mesh());
        CreateVerts();
        vector<glm::vec2> defuv;
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
//        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
//        glBindTexture(GL_TEXTURE_2D,0);
        glBindVertexArray(glmesh->vao);
        glDrawElements(GL_TRIANGLES,54,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
//        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }

    void SetTCoords(vector<glm::vec2> tcoords)
    {
        CreateTCoords(tcoords);

        glBindBuffer(GL_ARRAY_BUFFER, glmesh->buffers[1]->Id);
        glBufferData(GL_ARRAY_BUFFER, 16*sizeof(tex_coords->data[0]), &tex_coords->data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetRatio(glm::vec2 size,int margin=1)
    {
        m_margin=(float)margin;

        ratio.x=1.f/size.x;
        ratio.y=1.f/size.y;

        CreateVerts();

        glmesh->buffers[0]=pos;
        glmesh->UploadBuffers();
    }
};
