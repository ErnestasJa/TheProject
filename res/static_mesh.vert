#version 330

uniform mat4 MVP;

layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;
layout (location=2) in vec3 normal;
layout (location=3) in vec4 vtangent;
layout (location=4) in vec4 vbones;
layout (location=5) in vec4 vweights;
out vec2 UV;

void main(void)
{
   vec4 mpos= vec4(pos,1);
   gl_Position = MVP * mpos;
   UV = tex;
   //mat3 madjtrans = mat3(cross(m[1].xyz, m[2].xyz), cross(m[2].xyz, m[0].xyz), cross(m[0].xyz, m[1].xyz));
   //vec3 mnormal = normal * madjtrans;
   //vec3 mtangent = vtangent.xyz * madjtrans; // tangent not used, just here as an example
   //vec3 mbitangent = cross(mnormal, mtangent) * vtangent.w; // bitangent not used, just here as an example
}
