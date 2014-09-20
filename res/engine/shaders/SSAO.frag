#version 330

uniform sampler2D g_buffer_diff;
uniform sampler2D g_buffer_norm;
uniform sampler2D g_buffer_pos;
uniform sampler2D g_random;
uniform sampler2D g_depth;

uniform mat4 MV;

const float g_screen_size=1280*768;
const float random_size=1280*768;
const float g_sample_rad=0.006;
const float g_intensity=1.38;
const float g_scale=0.000002;
const float g_bias=0.07;

in vec4 _pos;
in vec2 _uv;

out vec4 fragColor;

vec3 getPosition(vec2 uv)
{
return vec3(MV*texture2D(g_buffer_pos,uv));
}

vec3 getNormal(vec2 uv)
{
return normalize(texture2D(g_buffer_norm, uv).xyz * 2.0f - 1.0f);
}

vec2 getRandom(vec2 uv)
{
return normalize(texture2D(g_random, g_screen_size * uv / random_size).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(vec2 tcoord,in vec2 uv, in vec3 p, in vec3 cnorm)
{
vec3 diff = getPosition(tcoord + uv) - p;
vec3 v = normalize(diff);
float d = length(diff)*g_scale;
return max(0.0, dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main()
{
vec2 vecs[4];
vecs[0]= vec2(1,0);
vecs[1]= vec2(-1,0);
vecs[2]= vec2(0,1);
vecs[3]= vec2(0,-1);

vec3 p = getPosition(_uv);
vec3 n = getNormal(_uv);
vec2 rand = getRandom(_uv);

float ao = 0.0f;
float rad = g_sample_rad/p.z;

//**SSAO Calculation**//
int iterations = 4;
for (int j = 0; j < iterations; ++j)
{
  vec2 coord1 = reflect(vecs[j],rand)*rad;
  vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
			  coord1.x*0.707 + coord1.y*0.707);
  
  ao += doAmbientOcclusion(_uv,coord1*0.25, p, n);
  ao += doAmbientOcclusion(_uv,coord2*0.5, p, n);
  ao += doAmbientOcclusion(_uv,coord1*0.75, p, n);
  ao += doAmbientOcclusion(_uv,coord2, p, n);
}
ao/=iterations*4.0;
//**END**//

//Do stuff here with your occlusion value Ã¢aoÃ¢: modulate ambient lighting, write it to a buffer for later //use, etc.
fragColor=texture2D(g_buffer_diff, _uv);//*(1-ao);
}