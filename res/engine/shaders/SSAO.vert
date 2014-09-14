sampler2D g_buffer_norm;
sampler2D g_buffer_pos;
sampler2D g_random;

uniform float random_size;
uniform float g_sample_rad;
uniform float g_intensity;
uniform float g_scale;
uniform float g_bias;

struct PS_INPUT
{
float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
float4 color : COLOR0;
};

vec3 getPosition(in float2 uv)
{
return tex2D(g_buffer_pos,uv).xyz;
}

float3 getNormal(in float2 uv)
{
return normalize(tex2D(g_buffer_norm, uv).xyz * 2.0f - 1.0f);
}

float2 getRandom(in float2 uv)
{
return normalize(tex2D(g_random, g_screen_size * uv / random_size).xy * 2.0f - 1.0f);
}

float doAmbientOcclusion(in float2 tcoord,in float2 uv, in float3 p, in float3 cnorm)
{
float3 diff = getPosition(tcoord + uv) - p;
const float3 v = normalize(diff);
const float d = length(diff)*g_scale;
return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

PS_OUTPUT main(PS_INPUT i)
{
PS_OUTPUT o = (PS_OUTPUT)0;

o.color.rgb = 1.0f;
const float2 vec[4] = {float2(1,0),float2(-1,0),
			float2(0,1),float2(0,-1)};

float3 p = getPosition(i.uv);
float3 n = getNormal(i.uv);
float2 rand = getRandom(i.uv);

float ao = 0.0f;
float rad = g_sample_rad/p.z;

//**SSAO Calculation**//
int iterations = 4;
for (int j = 0; j < iterations; ++j)
{
  float2 coord1 = reflect(vec[j],rand)*rad;
  float2 coord2 = float2(coord1.x*0.707 - coord1.y*0.707,
			  coord1.x*0.707 + coord1.y*0.707);
  
  ao += doAmbientOcclusion(i.uv,coord1*0.25, p, n);
  ao += doAmbientOcclusion(i.uv,coord2*0.5, p, n);
  ao += doAmbientOcclusion(i.uv,coord1*0.75, p, n);
  ao += doAmbientOcclusion(i.uv,coord2, p, n);
}
ao/=(float)iterations*4.0;
//**END**//

//Do stuff here with your occlusion value Ã¢aoÃ¢: modulate ambient lighting, write it to a buffer for later //use, etc.
return o;
}