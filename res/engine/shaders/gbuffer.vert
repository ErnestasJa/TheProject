#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

layout (location=0) in vec3 pos;
layout (location=6) in vec4 col;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vData
{
    vec4 color;
	vec4 pos;
}vertex;

void main()
{
	vec4 _col=col/255.0;
	mat4 MVP = P*V*M;
	vertex.color=_col;
	vertex.pos=V*M*vec4(pos,1.0);
	gl_Position = MVP * vec4(pos,1);
}