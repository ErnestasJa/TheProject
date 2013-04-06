#include <stdio.h>
#include "shader.h"
#include "texture.h"
#include "GL/glfw.h"

const char vsh[]=R"(#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uvcoords;

out vec2 uv;

void main()
{
    gl_Position = position;
	uv = uvcoords;
}

)";

const char fsh[]=
R"(#version 330

uniform sampler2D tex, tex2;

in vec2 uv;
out vec4 outputColor;

void main()
{
   outputColor = vec4(texture( tex, uv ).rgb - texture( tex2, uv ).rgb,1);
}
)";

const float vertexPositions[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	 1.0f, 1.0f, 0.0f
};

const float vertexUV[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,

    0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

const uint8_t tex[]=
{
	255, 0, 0, 255,
	0, 255, 0, 255,
	0, 0, 255, 255,
	255, 255, 255, 255,
};

uint32_t vbo, uv_bo, vao;


void InitializeVertexBuffer()
{
	///vao
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	///vbo
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(1);
	///uv_bo
	glGenBuffers(1,&uv_bo);
	glBindBuffer(GL_ARRAY_BUFFER, uv_bo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexUV), vertexUV, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

#include "tgaloader.h"
#include <fstream>
texture * load_tex(std::string file)
{
    std::ifstream rfile(file);

    if(!rfile.is_open())
    {
        printf("Could not open file\n");
        return NULL;
    }

    rfile.seekg(0, std::ios::end);
    uint32_t size = rfile.tellg();
    rfile.seekg(0);


    tgaloader l;
    char * buf = new char[size];
    rfile.read(buf,size);
    return l.generate(buf,size);
}

int main()
{
	printf ( "Hello world!\n" );
	glxwInit();

	if ( !glfwInit() )
	{
		printf ( "glfw init failed!\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	if ( !glfwOpenWindow ( 640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW ) )
	{
		printf ( "glfw open window failed!\n" );
		return -1;
	}


	binding tex_binding[]={{"tex",0},{"tex2",1},{"",-1}};

	shader sh("default",vsh,fsh,nullptr,tex_binding);
	sh.compile();
	sh.link();
	sh.set();

    texture * tex = load_tex("test.tga");
    texture * tex2 = load_tex("test2.tga");

	if(tex2)
    {
        tex2->set(1);
    }

    if(tex)
    {
        tex->set(0);
    }


	/* Development going wild. or is it? */
	glClearColor ( 0.2f,0.2f,0.4f,1.0f );

	InitializeVertexBuffer();

	/* Loop until the user closes the window */
	while ( glfwGetWindowParam ( GLFW_OPENED ) &&!glfwGetKey ( GLFW_KEY_ESC ) )
	{
		/* Render here */
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,6);
		glBindVertexArray(0);

		/* Swap front and back buffers and process events */
		glfwSwapBuffers();
	}

	return 0;
}
