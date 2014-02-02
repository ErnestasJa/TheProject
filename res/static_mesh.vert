#version 330

uniform mat4 MVP;
uniform mat4 MV;
uniform mat3 N;
uniform vec3 light_position;
uniform vec3 camera_position;

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 normal;
layout (location=3) in vec4 vtangent;
layout (location=4) in vec4 vbones;
layout (location=5) in vec4 vweights;

smooth out vec4 color;
out vec2 UV;

const vec3 diffuse_color  = vec3(1,1,1);
const vec3 specular_color = vec3(0.2,0.2,0.2);
const vec4 ambient_color = vec4(0.6,0.6,0.6,1);
const float shininess = 0.08f;

void main(void)
{
	vec4 vEyeSpaceLightPosition = MV*vec4(light_position,1);
	vec4 vEyeSpacePosition = MV*vec4(pos,1);
	vec3 vEyeSpaceNormal = normalize(N*normal);
	vec3 L = normalize(vEyeSpaceLightPosition.xyz - vEyeSpacePosition.xyz);
	vec3 V = normalize(camera_position.xyz - vEyeSpacePosition.xyz);
	vec3 H = normalize(L+V);
	float diffuse = max(0, dot(vEyeSpaceNormal, L));
	float specular = max(0, pow(dot(vEyeSpaceNormal, H),shininess));
	color = ambient_color + diffuse*vec4(diffuse_color,1) + specular*vec4(specular_color, 1);
	
	gl_Position = MVP * vec4(pos,1);
	UV = tex;
}