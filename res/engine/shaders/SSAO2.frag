#version 330

in vec4 _pos;
in vec2 _uv;

uniform sampler2D g_buffer_diff;
uniform sampler2D g_depth;
uniform sampler2D g_buffer_norm;
uniform sampler2D g_buffer_pos;
uniform sampler2D g_random;

uniform float Time = 0;

uniform vec2 TexelSize=vec2(1,1);
uniform float OccluderBias = 0.375;
uniform float SamplingRadius = 11.0;
uniform vec2 Attenuation = vec2(1,1);

uniform bool ApplyVignette = false;

out vec4 FragColor;
 
// Mostly cribbed from http://devmaster.net/posts/3095/shader-effects-screen-space-ambient-occlusion
float SamplePixels (vec3 srcPosition, vec3 srcNormal, vec2 _uv)
{
    // Get the 3D position of the destination pixel
    vec3 dstPosition = texture(g_buffer_pos, _uv).xyz;

    // Calculate ambient occlusion amount between these two points
    // It is simular to diffuse lighting. Objects directly above the fragment cast
    // the hardest shadow and objects closer to the horizon have minimal effect.
    vec3 positionVec = dstPosition - srcPosition;

    float bias = OccluderBias;
    float z = texture(g_buffer_pos, _uv).w;
    bias += z * 0.5;

    float intensity = max(dot(normalize(positionVec), srcNormal) - bias, 0.0);

    // Attenuate the occlusion, similar to how you attenuate a light source.
    // The further the distance between points, the less effect AO has on the fragment.
    float dist;
    dist = length(positionVec) / SamplingRadius;
    float attenuation = 1.0 / (Attenuation.x + (Attenuation.y * dist));
    
    return intensity * attenuation;
}

// Mostly cribbed from http://devmaster.net/posts/3095/shader-effects-screen-space-ambient-occlusion
float ComputeOcclusion(vec2 tc, vec3 srcNormal)
{
    // Get position and normal vector for this fragment
    vec3 srcPosition = texture(g_buffer_pos, tc).xyz;
    vec2 randVec = normalize(texture(g_random, tc*20).xy * 2.0 - 1.0);
    float srcDepth = texture(g_depth, tc).r;
    
    // The following variable specifies how many pixels we skip over after each
    // iteration in the ambient occlusion loop. We can't sample every pixel within
    // the sphere of influence because that's too slow. We only need to sample
    // some random pixels nearby to apprxomate the solution.
    //
    // Pixels far off in the distance will not sample as many pixels as those close up.
    float kernelRadius = SamplingRadius * (1.0 - srcDepth);
    
    // Sample neighbouring pixels
    vec2 kernel[4];
    kernel[0] = vec2(0.0, 1.0); // top
    kernel[1] = vec2(1.0, 0.0); // right
    kernel[2] = vec2(0.0, -1.0);    // bottom
    kernel[3] = vec2(-1.0, 0.0);    // left
    
    const float Sin45 = 0.707107;   // 45 degrees = sin(PI / 4)
    
    // Sample from 16 pixels, which should be enough to appromixate a result. You can
    // sample from more pixels, but it comes at the cost of performance.
    float occlusion = 0.0;
    for (int i = 0; i < 4; ++i)
    {
        vec2 k1 = reflect(kernel[i], randVec);
        vec2 k2 = vec2(k1.x * Sin45 - k1.y * Sin45,
                k1.x * Sin45 + k1.y * Sin45);
        k1 *= TexelSize;
        k2 *= TexelSize;
        
        occlusion += SamplePixels(srcPosition, srcNormal, tc + k1 * kernelRadius);
        occlusion += SamplePixels(srcPosition, srcNormal, tc + k2 * kernelRadius * 0.75);
        occlusion += SamplePixels(srcPosition, srcNormal, tc + k1 * kernelRadius * 0.5);
        occlusion += SamplePixels(srcPosition, srcNormal, tc + k2 * kernelRadius * 0.25);
    }
    
    // Average and clamp ambient occlusion
    occlusion /= 16.0;
    return clamp(occlusion, 0.0, 1.0);
}

void main()
{
    vec2 tc = _uv;

    vec4 c = texture(g_buffer_diff, tc);

    //bool bkgd = (c.a == 0);
    c = mix(c,vec4(1),0.55); // brighten up for pastel-like colors

    vec3 n = texture(g_buffer_norm, tc).rgb;

    // The geometry has inconsistent normals...
    // So flip 'em if necessary
    //if (dot(n,vec3(0,0,1)) < 0)
    //    n = -n;

    float ov = 1 - ComputeOcclusion(tc, n);
    ov = ov*ov*ov;
    FragColor = ov*c;

    //if (bkgd) {
    //    FragColor = vec4(0.627, 0.322, 0.176, 1);
    //}

    if (ApplyVignette) {
        float d = distance(tc, vec2(0.5));
        FragColor.rgb *= 1 - 0.75 * d;
    }
}