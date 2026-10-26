#pragma once

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct Material
{
    float2 tiling;
    float2 offset;
    
};

struct Light
{
    float3 posiiton;
    uint type;
    float3 direction;
    float paddng1;
    float3 color;
    float intensity;
    float3 ambient;
    float paddng2;
};