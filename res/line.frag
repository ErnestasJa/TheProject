
#version 330
uniform sampler2D tex;
out vec4 FragColor;
in vec4 col;

void main()
{
    FragColor = col;
}
