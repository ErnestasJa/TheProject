#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

in vec4 color;

out vec4 fragcolor;

void main(){
	fragcolor  = color;
}
