#pragma once

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct Material
{
    float4 color;
    float2 tiling;
    float2 offset;
    float shininess;
};

struct Light
{
    float3 posiiton;
    uint type;
    float3 direction;
    float innerCutoff;
    float3 color;
    float intensity;
    float3 ambient;
    float outerCutoff;
};

float3 getLightVector(Light light, float3 position, out float attenuation)
{
    float3 L;
    attenuation = 1.0f;
    switch (light.type)
    {
        case LIGHT_DIRECTIONAL:
            L = normalize(-light.direction);
            break;
        case LIGHT_POINT:
            L = normalize(light.posiiton - position);
            break;
        case LIGHT_SPOT:
            L = normalize(light.posiiton - position);
            attenuation = smoothstep(light.outerCutoff, light.innerCutoff, dot(L, light.direction));
            break;
    }
    return L;
    
}