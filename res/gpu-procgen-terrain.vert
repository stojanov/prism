#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aClr;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;

uniform mat4 projectedView;
uniform mat4 transform;

uniform int layer_count;
uniform int using_simplex[10];
uniform float noise_scale[10];
uniform float noise_scale_m[10];
uniform float noise_offset_x[10];
uniform float noise_offset_y[10];
uniform float noise_opacity[10];
uniform int noise_blendingmode[10];
uniform float elevation;

out vec4 WorldPos;
out vec3 Color;
out vec2 UV;
out vec3 Normal;
out float MaxHeight;

float blendMultiply(float base, float blend) {
	return base*blend;
}

float blendMultiply(float base, float blend, float opacity) {
	return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity));
}

float blendSubtract(float base, float blend) {
	return max(base+blend-1.0,0.0);
}

float blendSubtract(float base, float blend, float opacity) {
	return (blendSubtract(base, blend) * opacity + base * (1.0 - opacity));
}

float blendScreen(float base, float blend) {
	return 1.0-((1.0-base)*(1.0-blend));
}

float blendScreen(float base, float blend, float opacity) {
	return (blendScreen(base, blend) * opacity + base * (1.0 - opacity));
}

float blendOverlay(float base, float blend) {
	return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
}

float blendOverlay(float base, float blend, float opacity) {
	return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));
}

float blendAdd(float base, float blend, float opacity)
{
    return min(base + blend, 1.0) * opacity + base * (1.0 - opacity);
}

vec2 hash( vec2 p ) // replace this by something better
{
	p = vec2( dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float hash2(int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float smooth_noise(int x, int y, int samples)
{
    int k = 4;
    float total = hash2(x, y) / k;

    for (int i = 1; i <= samples; i++)
    {
        // Sides
        total += (
            hash2(x + i, y) +
            hash2(x - i, y) +
            hash2(x, y + i) +
            hash2(x, y - i)
        ) / k;

        k <<= 1;

        // Corners
        total += (
            hash2(x - i, y - i) +
            hash2(x + i, y - i) +
            hash2(x - i, y + i) +
            hash2(x + i, y + i)
        ) / k;
    }
    
    return total;
}

float linear_noise(float x, float y)
{
    int intX = int(x);
    int intY = int(y);
    float fracX = x - intX;
    float fracY = y - intY;

    float v0 = smooth_noise(intX, intY, 1);
    float v1 = smooth_noise(intX + 1, intY, 1);

    float v2 = smooth_noise(intX, intY + 1, 1);
    float v3 = smooth_noise(intX + 1, intY + 1, 1);

    float y1 = mix(v0, v1, fracX);
    float y2 = mix(v2, v3, fracX);

    return mix(y1, y2, fracY);
}

float simplex( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor( p + (p.x+p.y)*K1 );
    vec2  a = p - i + (i.x+i.y)*K2;
    float m = step(a.y,a.x); 
    vec2  o = vec2(m,1.0-m);
    vec2  b = a - o + K2;
	vec2  c = a - 1.0 + 2.0*K2;
    vec3  h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3  n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot( n, vec3(70.0) );
}

float fractal_simplex(in vec2 p, vec2 offset, float scale, int octaves)
{
    float total = 0;

    for (int i = 0; i < octaves - 1; i++)
    {
        float freq = pow(2, i);
        float amp = pow(0.5, i);

        vec2 po = (offset + (scale * p)) * freq;

        total += simplex(po) * amp;
    }

    return total;
}

float fractal_value(in vec2 p, vec2 offset, float scale, int octaves)
{
    float total = 0;

    for (int i = 0; i < octaves - 1; i++)
    {
        float freq = pow(2, i);
        float amp = pow(0.5, i);
        total += linear_noise((offset.x + (scale * p.x)) * freq, (offset.y + (scale * p.y)) * freq) * amp;
    }

    return total;
}

void main()
{
    float height = 0.f;

    for (int i = 0; i < layer_count; i++)
    {
        float noiseVal = 0.f;

        if (using_simplex[i] == 1)
        {
            noiseVal = fractal_simplex(aPos.xz, vec2(noise_offset_x[i], noise_offset_y[i]), noise_scale[i] * noise_scale_m[i], 7);
        } 
        else
        {
            noiseVal = fractal_value(aPos.xz, vec2(noise_offset_x[i], noise_offset_y[i]), noise_scale[i] * noise_scale_m[i], 7);  
        }
        
        switch (noise_blendingmode[i])
        {
            case 0:
                height = blendAdd(height, noiseVal, noise_opacity[i]);
            break;
            case 1:
                height = blendScreen(height, noiseVal, noise_opacity[i]);
            break;
            case 2:
                height = blendOverlay(height, noiseVal, noise_opacity[i]);
            break;
            case 3:
                height = blendSubtract(height, noiseVal, noise_opacity[i]);
            break;
            case 4:
                height = blendMultiply(height, noiseVal, noise_opacity[i]);
            break;
            default:
                height = blendAdd(height, noiseVal, noise_opacity[i]);
            break;
        }

    }

    height = clamp(height, -1.f, 1.f);
    
    float y = height * elevation;

    vec4 pos = vec4(aPos.x, aPos.y + y, aPos.z, 1.f);
    vec4 worldPos = transform * pos;

    gl_Position = projectedView * worldPos;
    Color = Color;
    UV = aUV;
    Normal = aNormal;
    WorldPos = pos;
    MaxHeight = elevation;
}