#version 330
uniform sampler2D tex;
uniform float alpha;
in vec2 UV;
out vec4 FragColor;
uniform vec3 C;
void main()
{
    FragColor = vec4(texture(tex,UV).rgb,texture(tex,UV).a*alpha);
}
