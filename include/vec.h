#ifndef VEC_H
#define VEC_H

#include <stdbool.h>

float radians(float degrees);

typedef int IVec3[3];

bool IVec3_equal(const IVec3 a, const IVec3 b);

typedef float Vec3[3];

// dst = src * scale
void Vec3_scale(Vec3 src, float scale, Vec3 dst);

// dst = src_a + src_b
void Vec3_add(Vec3 src_a, Vec3 src_b, Vec3 dst);

// dst = src_a - src_b
void Vec3_sub(Vec3 src_a, Vec3 src_b, Vec3 dst);

// dst = src_a x src_b
void Vec3_mul(Vec3 src_a, Vec3 src_b, Vec3 dst);

// dst = src / ||src||
void Vec3_normalize(Vec3 src, Vec3 dst);

// src_a * src_b
float Vec3_dot(Vec3 src_a, Vec3 src_b);

// ||src||^2
float Vec3_magSquared(Vec3 src);

// ||src||
float Vec3_mag(Vec3 src);

typedef float Mat4[4][4];

// dst = src_a * src_b
void Mat4_mul(Mat4 src_a, Mat4 src_b, Mat4 dst);

// Computes the view matrix.
void Mat4_lookAt(Vec3 camera, Vec3 target, Vec3 up, Mat4 dst);

// Computes the perspective projection matrix.
void Mat4_perspective(float fov, float aspect, float near, float far, Mat4 dst);

#endif
