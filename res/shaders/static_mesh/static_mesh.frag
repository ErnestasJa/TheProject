#version 330

//uniforms
uniform sampler2D texture0;
uniform vec3 camera_pos;
uniform vec3 light_pos;

//inputs
in vec2 UV;
smooth in vec3 vEyeSpaceNormal;
smooth in vec3 vEyeSpacePosition;

//outputs
layout(location=0) out vec4 FragColor;

//constants
const vec3 diffuse_color = vec3(1,1,1);
const vec3 specular_color = vec3(1,0,0);
const float shininess = 256.0f;

void main()
{
	vec4 tex_color = texture2D(texture0,UV);
	
	if(tex_color.a<0.2)
		discard;
	
	vec3 vEyeSpaceLightPosition=light_pos;
	vec3 vEyeSpaceCameraPosition=camera_pos;
	
	vec3 N = normalize(vEyeSpaceNormal);
	vec3 L = normalize(vEyeSpaceLightPosition - vEyeSpacePosition);
	vec3 V = normalize(vEyeSpaceCameraPosition - vEyeSpacePosition);
	vec3 H = normalize(L+V);
	
	float diffuse = max(0, dot(N, L));
	float specular = max(0, pow(dot(N, H), shininess));
	
	FragColor = diffuse*vec4(diffuse_color,1) + specular * vec4(specular_color,1);
    FragColor *= tex_color;
}
