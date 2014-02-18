#VS
#version 420

layout (location=0) in vec3 pos;
layout (location=1) in vec3 norm;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 col;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 st;

out vec3 pos_eye,norm_eye,pos_world,norm_world;

out vec4 color;


void main () {
    st=uv;
    color=col;

    norm_world = (M * vec4(norm, 0.0)).xyz;
    pos_world = vec3(M * vec4(pos,1.0));
    pos_eye = vec3 (V * M * vec4 (pos, 1.0));
    norm_eye = vec3 (V * M * vec4 (norm, 0.0));

    //P *VM
    gl_Position = P * vec4 (pos_eye, 1.0);
}

#FS
#version 420

layout (binding = 0) uniform sampler2D tex;
layout (binding = 1) uniform sampler2D tex_spec;

uniform mat4 V;

in vec2 st;

in vec3 pos_eye,norm_eye,pos_world,norm_world;

in vec4 color;

struct PointLight {
    //base properties
    //color
    vec3 Color;
    //intensities
    vec3 Ambience;
    vec3 Diffusion;
    vec3 Specularity;
    //pos
    vec3 Position;

    //Light power
    float Power;
    //specular exponent
    float specexp;
};

PointLight PointLights[3];

vec4 CalcPointLight(PointLight L,vec3 SpecMap)
{
    // ambient intensity
	vec3 Ambient = L.Color * L.Ambience;

	// diffuse intensity
    //keep the light in the world space D:
    vec3 distance_to_light_world = L.Position - pos_world;
    vec3 direction_to_light_world = normalize (distance_to_light_world);
    float dot_prod = dot (direction_to_light_world,norm_world);
    dot_prod = max (dot_prod, 0.0);
    vec3 Diffuse = L.Color * L.Diffusion * dot_prod; // final diffuse intensity

    //gimme a specular mapped reflection
    vec3 light_position_eye = vec3 (V * vec4 (L.Position, 1.0));
    vec3 distance_to_light_eye = light_position_eye - pos_eye;
    float deye=length(distance_to_light_eye);
    deye=deye;
    vec3 direction_to_light_eye = normalize (distance_to_light_eye);

    vec3 reflection_eye = reflect (-direction_to_light_eye, norm_eye);
    vec3 surface_to_viewer_eye = normalize (-pos_eye);
    float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
    dot_prod_specular = max (dot_prod_specular, 0.0);
    float specular_factor = pow (dot_prod_specular, L.specexp);
    vec3 Specular = L.Specularity * SpecMap * specular_factor;//*1./deye; // final specular intensity

    //attenuation
    float dist=length(distance_to_light_world);
    float atten=1./L.Power+1./L.Power*dist+0.1/L.Power*dist*dist;

	return vec4(Specular+Diffuse+Ambient,1.0)/atten;
}

out vec4 frag_colour;
vec4 TotalLight=vec4(0);
void main () {
    PointLights[0].Color = vec3(0.75,0.5,0.0);
    PointLights[0].Diffusion = vec3(0.0);
    PointLights[0].Ambience = vec3(0);
    PointLights[0].Specularity = vec3(1);
    PointLights[0].Position = vec3(0.0,1.0,0.0);
    PointLights[0].Power=10;
    PointLights[0].specexp=100.0;

    PointLights[1].Color = vec3(0.7,0.0,0.7);
    PointLights[1].Diffusion = vec3(0.0);
    PointLights[1].Ambience = vec3(0);
    PointLights[1].Specularity = vec3(1);
    PointLights[1].Position = vec3(0.0,4.0,4.0);
    PointLights[1].Power=10.0;
    PointLights[1].specexp=1.0;

    PointLights[2].Color = vec3(0.0,0.7,0.7);
    PointLights[2].Diffusion = vec3(0.0);
    PointLights[2].Ambience = vec3(0);
    PointLights[2].Specularity = vec3(1);
    PointLights[2].Position = vec3(0.0,4.0,-4.0);
    PointLights[2].Power=10.0;
    PointLights[2].specexp=1.0;

    for(int i=0; i<3; i++)
    {
        TotalLight+=CalcPointLight(PointLights[i],texture(tex_spec,st).rgb);
    }

    frag_colour = TotalLight*texture (tex,st);
}

