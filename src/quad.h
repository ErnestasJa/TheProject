#pragma once

#include "mesh.h"
#include "shader.h"
#include "precomp.h"




class quad
{
public:
    glm::vec3 pos[4];
    glm::vec2 tex_coords[4];
    uint32_t indices[6];

    mesh glmesh;

    static shader quad_shader_textured;

    quad()
    {
        pos[0]=glm::vec3(-1,-1,0);
        pos[1]=glm::vec3(1,-1,0);
        pos[2]=glm::vec3(1,1,0);
        pos[3]=glm::vec3(-1,1,0);

        tex_coords[0] = glm::vec2(0,0);
        tex_coords[1] = glm::vec2(1,0);
        tex_coords[2] = glm::vec2(1,1);
        tex_coords[3] = glm::vec2(0,1);

        indices[0]=0;
        indices[1]=1;
        indices[2]=3;

        indices[3]=3;
        indices[4]=1;
        indices[5]=2;

        glmesh.buffers.resize(3);
    }

    bool generate()
    {
        glGenVertexArrays(1,&glmesh.vao);

        glBindVertexArray(glmesh.vao);

        glGenBuffers(1, &glmesh.buffers[0]);
        glBindBuffer(GL_ARRAY_BUFFER, glmesh.buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, 4*sizeof(pos[0]), pos, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &glmesh.buffers[1]);
        glBindBuffer(GL_ARRAY_BUFFER, glmesh.buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, 4*sizeof(tex_coords[0]), &tex_coords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1,&glmesh.buffers[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,glmesh.buffers[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,6*sizeof(uint32_t),&indices[0],GL_STATIC_DRAW);

        glBindVertexArray(0);

        return true;
    }

    void draw()
    {
        glBindVertexArray(glmesh.vao);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
};
