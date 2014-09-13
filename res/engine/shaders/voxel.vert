#version 330

uniform mat4 mvp;
layout (location=0) in vec3 pos;
layout (location=6) in vec4 col;

out vec4 _col;

void main(){
		//_col=vec4(1.0/255.0*col.x,1.0/255.0*col.y,1.0/255.0*col.z,1.0/255.0*col.w);
        gl_Position = mvp * vec4(pos,1);
}