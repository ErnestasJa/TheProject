#version 330

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 M;
uniform mat3 N;
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
	vEyeSpacePosition = (M*vec4(pos,1)).xyz;
	vEyeSpaceNormal = N*normal;
	
	gl_Position = MVP * vec4(pos,1);
	UV = tex;
}