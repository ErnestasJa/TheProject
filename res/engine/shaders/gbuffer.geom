#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform mat3 normMatrix;
 
in vData
{
    vec4 color;
	vec4 pos;
}vertex[];

out vec3 normal;
out vec4 color;
out vec4 position;

void main()
{
	vec3 calcNorm=normalize(normMatrix* cross(vertex[1].pos.xyz - vertex[0].pos.xyz, vertex[2].pos.xyz - vertex[0].pos.xyz))*0.5+vec3(0.5);
	normal = calcNorm;
	color = vertex[0].color;
	position = vertex[0].pos;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
	normal = calcNorm;
	color = vertex[1].color;
	position = vertex[1].pos;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
	normal = calcNorm;
	color = vertex[2].color;
	position = vertex[2].pos;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
