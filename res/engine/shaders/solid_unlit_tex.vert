#version 330

uniform mat4 mvp;
uniform sampler2D texture0;

layout (location=0) in vec3 pos;
layout (location=1) in vec2 uv;
layout (location=2) in vec3 normal;

smooth out vec3 _normal;
out vec2 _uv;

void main(){
		_uv = uv;
        _normal = normal;
        gl_Position = mvp * vec4(pos,1);
}