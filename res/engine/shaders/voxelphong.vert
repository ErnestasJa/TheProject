#version 330

// THE FOOD CHAIN GOES LIKE DIS
// Vert
// Geom
// Frag

layout (location=0) in vec3 pos;
layout (location=6) in vec4 col;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 position;

out vData
{
    vec4 color;
	vec4 pos;
}vertex;

void main()
{
  vec4 _col=vec4(col.x/255,col.y/255,col.z/255,col.w/255);
  position = M * vec4(pos,1);
  vertex.pos=position;
  vertex.color=_col;
  mat4 MVP = P*V*M;
  gl_Position = MVP * vec4(pos,1);
}