
#version 330

uniform sampler2D tex;

in vec2 UV;
smooth in vec4 color;

out vec4 FragColor;

void main()
{
    FragColor = color * texture2D(tex,UV);
}
