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
const vec3 specular_color = vec3(1,1,1);
const float shininess = 256.0f;

void light_func(
	out float diffuse,
	out float specular,
	in vec3 light_position,
	in vec3 camera_position,
	in vec3 vert_eye_space_position,
	in vec3 vert_eye_space_normal)
{
	vec3 vEyeSpaceLightPosition=light_pos;
	vec3 vEyeSpaceCameraPosition=camera_pos;

	vec3 N = normalize(vert_eye_space_normal);
	vec3 L = normalize(vEyeSpaceLightPosition - vert_eye_space_position);
	vec3 V = normalize(vEyeSpaceCameraPosition - vert_eye_space_position);
	vec3 H = normalize(L+V);
	
	diffuse = max(0, dot(N, L));
	specular = max(0, pow(dot(N, H), shininess));
}

void main()
{
	vec4 tex_color = texture2D(texture0,UV);
	
	if(tex_color.a<0.2)
		discard;
	
	float diffuse = 0.0;
	float specular = 0.0;

	light_func(diffuse, specular, light_pos, camera_pos, vEyeSpacePosition, vEyeSpaceNormal);
	
	FragColor = diffuse*vec4(diffuse_color,1) + specular * vec4(specular_color,1)+vec4(0.5,0.5,0.5,1);
    FragColor *= tex_color;
}
