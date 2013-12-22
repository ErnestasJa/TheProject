
#version 330
uniform sampler2D tex;
in vec2 UV;
out vec4 FragColor;

void main()
{
    //FragColor = texture2D(tex,UV);
	FragColor = vec4(1.0,0.0,0.0,1.0);
}
