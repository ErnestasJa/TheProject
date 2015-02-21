#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

layout (points) in;
layout (triangle_strip, max_vertices=32) out;

uniform mat4 V;
uniform mat4 P;
 
in vData
{
    vec4 color;
	vec4 pos;
}vertex[];

out vec4 color;
out vec4 pos;

void main()
{
//left
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,0.5,0.0));
EmitVertex();
EndPrimitive();

//right
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,-0.5,0.0));
EmitVertex();
EndPrimitive();

//top
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,-0.5,0.0));
EmitVertex();
EndPrimitive();

//bottom
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,0.5,0.0));
EmitVertex();
EndPrimitive();

//front
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(0.5,-0.5,0.5,0.0));
EmitVertex();
EndPrimitive();

//back
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,0.5,-0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,0.5,0.0));
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+vec4(-0.5,-0.5,-0.5,0.0));
EmitVertex();
EndPrimitive();
}
