#version 330

uniform mat4 MVP;
uniform vec4 color;

layout (location=0) in vec3 pos;

out vec2 UV;
out vec4 col;

void main(void)
{
   gl_Position = MVP * vec4(pos,1);
   col = color;
}
