#version 330

uniform mat4 MVP;
layout (location=0) in vec3 pos;
layout (location=6) in vec4 col;

out vec4 _col;
out vec3 _pos;
out vec3 _norm;

void main(){
	_pos=pos;
	_col=vec4(1.f/255.f*col.x,1.f/255.f*col.y,1.f/255.f*col.z,1.f/255.f*col.w);
    gl_Position = MVP * vec4(pos,1);
}