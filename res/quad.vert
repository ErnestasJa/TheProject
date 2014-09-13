#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;

out vec2 UV;

void main(void)
{
    UV = tex;
    gl_Position = vec4(pos,1);
}
