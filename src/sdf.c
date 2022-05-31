#include "perlin.h"
#include "sdf.h"

#include <math.h>

float perlinSDF(const Vec3 p)
{ 
    return perlin(p[0] * 0.01f, p[1] * 0.01f, p[2] * 0.01f)
    + 0.5f * perlin(p[0] * 0.05f, p[1] * 0.05f, p[2] * 0.05f)
    + 0.1f * perlin(p[0] * 0.1f, p[1] * 0.1f, p[2] * 0.1f);
} 

float caveSDF(const Vec3 p)
{ 
    float o1 = perlin(p[0] * 0.01f, p[1] * 0.01f, p[2] * 0.01f);

    return o1;
} 

float terrainSDF(const Vec3 p)
{ 
    float o1 = 50.0f * perlin(p[0] * 0.005f, 0.0f, p[2] * 0.005f);
    float o2 =  2.0f * perlin(p[0] * 0.050f, 0.0f, p[2] * 0.050f);
    float o3 =  1.0f * perlin(p[0] * 0.100f, 0.0f, p[2] * 0.100f);

    float surface = p[1] - o1 - o2 - o3;
    float cave = perlinSDF(p);

    float k  = 32.0f;
    float h = 0.5f - 0.5f * (cave - surface) / k;
    if (h < 0.0f)
    { 
        h = 0.0f;
    } 
    if (h > 1.0f)
    { 
        h = 1.0f;
    } 
    
    float a = cave * (1.0f - h) + surface * h;
    float b = k * h * (1.0f - h);
    
    return a + b;
} 
    
float sphereSDF(const float p[3])
{ 
    return sqrtf(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]) - 25.0f;
} 
