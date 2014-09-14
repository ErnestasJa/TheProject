#version 330
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex;

out vec4 _pos;
out vec2 _uv;

void main(void)
{
	_pos = vec4(pos,1);
    _uv = tex;
    gl_Position = vec4(pos,1);
}
