#version 330

// THE FOOD CHAIN GOES LIKE DIS
// Vert
// Geom
// Frag

in vec4 position;  // position of the vertex (and fragment) in world space
in vec3 normal; // normal of the vertex in world space
in vec4 color; // color of the vertex

layout (location = 0) out vec3 Gdiffuse;
layout (location = 1) out vec3 Gnormal;
layout (location = 2) out vec3 Gposition;
layout (location = 3) out vec3 Gtexcoord;

void main(){

	Gdiffuse = color.rgb;
	Gnormal = normal;
	Gposition = position.rgb;
	Gtexcoord = vec3(0);
}