#version 330

uniform mat4 mvp;
uniform mat4 m;
uniform mat3 n;
uniform vec3 light_pos;
uniform vec3 camera_pos;

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 normal;


out vec2 UV;
smooth out vec3 vEyeSpaceNormal;
smooth out vec3 vEyeSpacePosition;

void main(void)
{
	vEyeSpacePosition = (m*vec4(pos,1)).xyz;
	vEyeSpaceNormal = n*normal;
	
	gl_Position = mvp * vec4(pos,1);
	UV = tex;
}
