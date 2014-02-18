#version 330 core

///uniforms
uniform sampler2D tex;

///outputs
layout(location=0) out vec4 vFragColor;
in vec2 UV;

void main()
{ 
	vFragColor = texture(tex, UV);  
}