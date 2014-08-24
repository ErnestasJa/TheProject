#version 330

uniform mat4 mvp;
layout (location=0) in vec3 pos;
layout (location=6) in vec4 col;

out vec4 _col;

void main(){
		_col=col;
        gl_Position = mvp * vec4(pos,1);
}