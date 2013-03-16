#include <stdio.h>
#include "shader.h"

const char vsh[]=R"(#version 330

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}

)";

const char fsh[]=
R"(#version 330

out vec4 outputColor;

void main()
{
   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";

const float vertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

uint32_t vbo, vao;

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
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
    glBindVertexArray(0);
    
    
}

int main()
{
	printf ( "Hello world!\n" );
	printf ( "Shader:\n%s\n",vsh);
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

	shader sh("default",vsh,fsh);
	sh.compile();
	sh.link();
	sh.set();
	
	/* Development going wild. or is it? */
	glClearColor ( 0.2f,0.2f,0.4f,1.0f );
	
	InitializeVertexBuffer();

	/* Loop until the user closes the window */
	while ( glfwGetWindowParam ( GLFW_OPENED ) &&!glfwGetKey ( GLFW_KEY_ESC ) )
	{
		/* Render here */
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,3);
		glBindVertexArray(0);
		
		/* Swap front and back buffers and process events */
		glfwSwapBuffers();
	}

	return 0;
}
