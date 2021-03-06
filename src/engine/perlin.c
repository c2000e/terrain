#include "perlin.h"

#include <math.h>

const static u16 p[512] = {
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7,
    225, 140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,
      6, 148, 247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,
     35,  11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136,
    171, 168,  68, 175,  74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158,
    231,  83, 111, 229, 122,  60, 211, 133, 230, 220, 105,  92,  41,  55,  46,
    245,  40, 244, 102, 143,  54,  65,  25,  63, 161,   1, 216,  80,  73, 209,
     76, 132, 187, 208,  89,  18, 169, 200, 196, 135, 130, 116, 188, 159,  86,
    164, 100, 109, 198, 173, 186,   3,  64,  52, 217, 226, 250, 124, 123,   5,
    202,  38, 147, 118, 126, 255,  82,  85, 212, 207, 206,  59, 227,  47,  16,
     58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 119, 248, 152,   2,  44,
    154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9, 129,  22,  39, 253,
     19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104, 218, 246,  97,
    228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,  81,  51,
    145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157, 184,
     84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156,
    180,
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7,
    225, 140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,
      6, 148, 247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,
     35,  11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136,
    171, 168,  68, 175,  74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158,
    231,  83, 111, 229, 122,  60, 211, 133, 230, 220, 105,  92,  41,  55,  46,
    245,  40, 244, 102, 143,  54,  65,  25,  63, 161,   1, 216,  80,  73, 209,
     76, 132, 187, 208,  89,  18, 169, 200, 196, 135, 130, 116, 188, 159,  86,
    164, 100, 109, 198, 173, 186,   3,  64,  52, 217, 226, 250, 124, 123,   5,
    202,  38, 147, 118, 126, 255,  82,  85, 212, 207, 206,  59, 227,  47,  16,
     58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 119, 248, 152,   2,  44,
    154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9, 129,  22,  39, 253,
     19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104, 218, 246,  97,
    228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,  81,  51,
    145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157, 184,
     84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156,
    180
};

static f32 fade(f32 t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static f32 lerp(f32 t, f32 a, f32 b)
{
    return a + t * (b - a);
}

const static f32 g[16][3] = {
    {  1,  1,  0 },
    { -1,  1,  0 },
    {  1, -1,  0 },
    { -1, -1,  0 },
    {  1,  0,  1 },
    { -1,  0,  1 },
    {  1,  0, -1 },
    { -1,  0, -1 },
    {  0,  1,  1 },
    {  0, -1,  1 },
    {  0,  1, -1 },
    {  0, -1, -1 },
    {  1,  1,  0 },
    {  0, -1,  1 },
    { -1,  1,  0 },
    {  0, -1, -1 }
};

static f32 grad(u16 hash, f32 x, f32 y, f32 z)
{
    u16 h = hash & 15;
    return g[h][0] * x + g[h][1] * y + g[h][2] * z;
}

f32 perlin(f32 x, f32 y, f32 z)
{
    i32 X = (i32)floorf(x) & 255;
    i32 Y = (i32)floorf(y) & 255;
    i32 Z = (i32)floorf(z) & 255;

    x -= floorf(x);
    y -= floorf(y);
    z -= floorf(z);
    
    f32 u = fade(x);
    f32 v = fade(y);
    f32 w = fade(z);
    
    i32 a = p[p[p[X    ] + Y    ] + Z];
    i32 b = p[p[p[X + 1] + Y    ] + Z];
    i32 c = p[p[p[X    ] + Y + 1] + Z];
    i32 d = p[p[p[X + 1] + Y + 1] + Z];
    i32 e = p[p[p[X    ] + Y    ] + Z + 1];
    i32 f = p[p[p[X + 1] + Y    ] + Z + 1];
    i32 g = p[p[p[X    ] + Y + 1] + Z + 1];
    i32 h = p[p[p[X + 1] + Y + 1] + Z + 1];

    return lerp(w, lerp(v, lerp(u, grad(a, x    , y    , z    ),
                                   grad(b, x - 1, y    , z    )),
                           lerp(u, grad(c, x    , y - 1, z    ),
                                   grad(d, x - 1, y - 1, z    ))),
                   lerp(v, lerp(u, grad(e, x    , y    , z - 1),
                                   grad(f, x - 1, y    , z - 1)),
                           lerp(u, grad(g, x    , y - 1, z - 1),
                                   grad(h, x - 1, y - 1, z - 1))));
}
