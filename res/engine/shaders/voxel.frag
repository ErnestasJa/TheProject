#version 330

uniform mat4 proj;
uniform mat4 view;

in vec4 _col;
in vec3 _pos;
in vec3 _norm;

out vec4 fragcolor;

void main(){
	fragcolor = _col;
}