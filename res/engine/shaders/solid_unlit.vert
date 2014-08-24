#version 330

uniform mat4 mvp;
layout (location=0) in vec3 pos;

smooth out vec3 _normal;

void main(){
        gl_Position = mvp * vec4(pos,1);
}