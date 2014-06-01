#version 330

//uniforms
uniform sampler2D texture0;


//inputs
in vec2 UV;

//outputs
layout(location=0) out vec4 FragColor;

//constants

void main()
{
	vec4 tex_color = texture2D(texture0,UV);
    FragColor = tex_color;
}
