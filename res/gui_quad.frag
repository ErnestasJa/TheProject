#version 330
uniform sampler2D tex;
in vec2 UV;
out vec4 FragColor;
uniform vec3 C;
void main()
{
    FragColor = vec4(C,1);
}
