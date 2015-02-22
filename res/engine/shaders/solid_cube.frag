#version 330

in vec4 _col;
smooth in vec3 _pos;

out vec4 outColor;

void main()
{
  vec3 pm= mod(_pos+vec3(0.05,0.05,0.05),1.0);
  if( ( pm.x < 0.1 && ( pm.y<0.1||pm.z<0.1 ) )
	  || ( pm.y < 0.1 && ( pm.z < 0.1 ) ) )
	outColor = vec4(0.05,0.05,0.05,1);
  else
	outColor  = _col;
}