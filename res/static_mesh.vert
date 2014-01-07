#version 330

uniform mat4 MVP;

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 normal;
layout (location=3) in vec4 vtangent;
layout (location=4) in vec4 vbones;
layout (location=5) in vec4 vweights;
out vec2 UV;

void main(void)
{
   gl_Position = MVP * vec4(pos,1);
   UV = tex;
}
