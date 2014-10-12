#version 330

uniform mat4 proj;
uniform mat4 view;

in vec4 _col;
in vec3 _pos;
in vec3 _norm;

out vec4 fragcolor;

void main(){
  vec3 pm= mod(_pos+vec3(0.05,0.05,0.05),1.0);

  if( ( pm.x < 0.1 && ( pm.y<0.1||pm.z<0.1 ) )
	  || ( pm.y < 0.1 && ( pm.z < 0.1 ) ) )
	fragcolor = vec4(0.05,0.05,0.05,1);
  else
	fragcolor  = _col;
}