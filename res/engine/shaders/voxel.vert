#version 330

uniform mat4 mvp;
layout (location=0) in vec3 pos;
layout (location=2) in vec3 norm;
layout (location=6) in vec4 col;

out vec4 _col;
out vec3 _pos;
out vec3 _norm;

void main(){
	_pos=pos;
	_col=vec4(col.x/255,col.y/255,col.z/255,col.w/255);
	_norm=norm;
    gl_Position = mvp * vec4(pos,1);
}