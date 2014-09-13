#version 330

uniform mat4 proj;
uniform mat4 view;

in vec4 _col;
in vec3 _pos;
in vec3 _norm;

out vec4 fragcolor;

void main(){
	vec3 n = normalize(mat3(view) * _norm);      // convert normal to view space, u_vm (view matrix), is a rigid body transform.
	vec3 p = vec3(view * vec4(_pos,1));                   // position in view space
	vec3 v = normalize(-p);                       // eye vector
	float vdn = 1.0 - max(dot(v, n), 0.0);        // the rim contribution
 
	fragcolor.a = 1.0*_col.a;
	fragcolor.rgb = _col.rgb*vec3(smoothstep(0.6, 1.0, vdn));
}