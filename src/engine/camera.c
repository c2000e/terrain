#include "camera.h"

#include <math.h>
#include <stdlib.h>

Camera *Camera_make(Transform *t)
{
    Camera *camera = malloc(sizeof *camera);
    camera->transform = t;
    Camera_defaultSettings(camera);
    Camera_updateMatrix(camera);
    Camera_updateFrustum(camera);
    return camera;
}

void Camera_free(Camera *camera)
{
    if (camera)
    {
        free(camera);
    }
}

void Camera_defaultSettings(Camera* camera)
{
    camera->fovy = 90.0f;
    camera->aspect = 1.0f;
    camera->near = 0.1f;
    camera->far = 200.0f;

    camera->near_h = 2 * tan(camera->fovy / 2) * camera->near;
    camera->near_w = camera->near_h * camera->aspect;

    camera->far_h = 2 * tan(camera->fovy / 2) * camera->far;
    camera->far_w = camera->far_h * camera->aspect;
}

void Camera_updateMatrix(Camera *camera)
{
    Vec3 camera_target;
    Vec3_add(
            camera->transform->position,
            camera->transform->forward,
            camera_target
    );

    Mat4 view;
    Mat4_lookAt(
            camera->transform->position,
            camera_target,
            camera->transform->up,
            view
    );

    Mat4 projection;
    Mat4_perspective(
            radians(camera->fovy),
            camera->aspect,
            camera->near,
            camera->far,
            projection
    );

    Mat4_mul(projection, view, camera->matrix);
}

void Camera_updateFrustum(Camera *camera)
{
    // Near plane
    camera->frustum[0].n[0] = camera->transform->forward[0];
    camera->frustum[0].n[1] = camera->transform->forward[1];
    camera->frustum[0].n[2] = camera->transform->forward[2];
    Vec3_scale(
            camera->transform->forward,
            camera->near,
            camera->frustum[0].p
    );
    Vec3_add(
            camera->transform->position,
            camera->frustum[0].p,
            camera->frustum[0].p
    );

    // Far plane
    camera->frustum[1].n[0] = -camera->transform->forward[0];
    camera->frustum[1].n[1] = -camera->transform->forward[1];
    camera->frustum[1].n[2] = -camera->transform->forward[2];
    Vec3_scale(
            camera->transform->forward,
            camera->far,
            camera->frustum[1].p
    );
    Vec3_add(
            camera->transform->position,
            camera->frustum[1].p,
            camera->frustum[1].p
    );

    // Used to store vector in plane
    Vec3 a;

    // Top plane
    Vec3_scale(
            camera->transform->up,
            camera->near_h / 2,
            camera->frustum[2].p
    );
    Vec3_add(
            camera->frustum[0].p,
            camera->frustum[2].p,
            camera->frustum[2].p
    );

    Vec3_sub(
            camera->frustum[2].p,
            camera->transform->position,
            a
    );

    Vec3_mul(
            a,
            camera->transform->right,
            camera->frustum[2].n
    );
    Vec3_normalize(
            camera->frustum[2].n,
            camera->frustum[2].n
    );

    // Bottom plane
    Vec3_scale(
            camera->transform->up,
            camera->near_h / 2,
            camera->frustum[3].p
    );
    Vec3_sub(
            camera->frustum[0].p,
            camera->frustum[3].p,
            camera->frustum[3].p
    );

    Vec3_sub(
            camera->frustum[3].p,
            camera->transform->position,
            a
    );

    Vec3_mul(
            camera->transform->right,
            a,
            camera->frustum[3].n
    );
    Vec3_normalize(
            camera->frustum[3].n,
            camera->frustum[3].n
    );

    // Left plane
    Vec3_scale(
            camera->transform->right,
            camera->near_w / 2,
            camera->frustum[4].p
    );
    Vec3_sub(
            camera->frustum[0].p,
            camera->frustum[4].p,
            camera->frustum[4].p
    );

    Vec3_sub(
            camera->frustum[4].p,
            camera->transform->position,
            a
    );

    Vec3_mul(
            a,
            camera->transform->up,
            camera->frustum[4].n
    );
    Vec3_normalize(
            camera->frustum[4].n,
            camera->frustum[4].n
    );

    // Right plane
    Vec3_scale(
            camera->transform->right,
            camera->near_w / 2,
            camera->frustum[5].p
    );
    Vec3_add(
            camera->frustum[0].p,
            camera->frustum[5].p,
            camera->frustum[5].p
    );

    Vec3_sub(
            camera->frustum[5].p,
            camera->transform->position,
            a
    );

    Vec3_mul(
            camera->transform->up,
            a,
            camera->frustum[5].n
    );
    Vec3_normalize(
            camera->frustum[5].n,
            camera->frustum[5].n
    );

}

static f32 planeSDF(const Plane *p, const Vec3 r)
{
    return Vec3_dot(p->n, r) - Vec3_dot(p->n, p->p);
}

b8 Camera_sphereInFrustum(const Camera *camera, Vec3 p, f32 r)
{
    for (i32 i = 0; i < 6; i++)
    {
        f32 distance = planeSDF(&camera->frustum[i], p);
        if (distance < -r)
        {
            return false;
        }
        else if (distance < r)
        {
            return true;
        }
    }
    return true;
}

