#version 330

// THE FOOD CHAIN GOES LIKE DIS
// Vert
// Geom
// Frag

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform mat3 m_3x3_inv_transp;
 
in vData
{
    vec4 color;
}vertex[];

out fData
{
    vec3 normal;
    vec4 color;
}frag; 
 
 void main()
{
	vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 tnormal=normalize(cross(ab, ac));
	vec3 onormal=normalize(m_3x3_inv_transp*tnormal);
	
	for (int i = 0; i < 3; i++)
    {
		frag.color = vertex[i].color;
		frag.normal = onormal;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}