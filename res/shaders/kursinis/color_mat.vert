#version 330

uniform mat4 mvp;

layout (location=0) in vec3 pos;


void main(void)
{
	gl_Position = mvp * vec4(pos,1);
}
