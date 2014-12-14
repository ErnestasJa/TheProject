#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

in vec4 position;
in vec3 normal;
in vec4 color;

layout (location = 0) out vec3 Gdiffuse;
layout (location = 1) out vec3 Gnormal;
layout (location = 2) out vec3 Gposition;
layout (location = 3) out vec3 Gtexcoord;

void main(){

	Gdiffuse = color.rgb;
	Gnormal = normal;
	Gposition = position.rgb;
}
