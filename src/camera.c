#include "camera.h"

#include <math.h>
#include <stdlib.h>

Camera *Camera_make(float x, float y, float z, float yaw, float pitch)
{
    Camera *camera = malloc(sizeof *camera);
    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;
    camera->yaw = yaw;
    camera->pitch = pitch;
    Camera_defaultSettings(camera);
    Camera_updateVectors(camera);
    Camera_updateMatrix(camera);
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
    camera->world_up[0] = 0.0f;
    camera->world_up[1] = 1.0f;
    camera->world_up[2] = 0.0f;
    camera->fovy = 90.0f;
    camera->aspect = 1.0f;
    camera->near = 0.1f;
    camera->far = 200.0f;
    camera->speed = 0.1f;
    camera->sensitivity = 0.3f;

    camera->near_h = 2 * tan(camera->fovy / 2) * camera->near;
    camera->near_w = camera->near_h * camera->aspect;

    camera->far_h = 2 * tan(camera->fovy / 2) * camera->far;
    camera->far_w = camera->far_h * camera->aspect;
}

void Camera_updateRotation(Camera* camera, float dx, float dy)
{
    camera->yaw += dx * camera->sensitivity;

    camera->pitch += dy * camera->sensitivity;
    if (camera->pitch > 85.0f) camera->pitch = 85.0f;
    if (camera->pitch < -85.0f) camera->pitch = -85.0f;
}

void Camera_updatePosition(Camera* camera, float x, float y, float z)
{
    Vec3 movement_x;
    Vec3_scale(camera->right, x, movement_x);

    Vec3 movement_y;
    Vec3_scale(camera->up, y, movement_y);

    Vec3 movement_z;
    Vec3_scale(camera->front, z, movement_z);

    Vec3 movement;
    Vec3_add(movement_x, movement_y, movement);
    Vec3_add(movement_z, movement, movement);
    Vec3_normalize(movement, movement);
    Vec3_scale(movement, 0.1f, movement);

    Vec3_add(camera->position, movement, camera->position);
}

void Camera_updateVectors(Camera* camera)
{
    Vec3 front;
    front[0] = cosf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    front[1] = sinf(radians(camera->pitch));
    front[2] = sinf(radians(camera->yaw)) * cosf(radians(camera->pitch));
    Vec3_normalize(front, camera->front);

    Vec3_mul(camera->front, camera->world_up, camera->right);
    Vec3_normalize(camera->right, camera->right);

    Vec3_mul(camera->right, camera->front, camera->up);
    Vec3_normalize(camera->up, camera->up);
}

void Camera_updateMatrix(Camera* camera)
{
    Vec3 camera_target;
    Vec3_add(camera->position, camera->front, camera_target);

    Mat4 view;
    Mat4_lookAt(camera->position, camera_target, camera->up, view);

    Mat4 projection;
    Mat4_perspective(radians(camera->fovy), camera->aspect, camera->near,
            camera->far, projection);

    Mat4_mul(projection, view, camera->matrix);
}

void Camera_move(Camera *camera, UserInput *input)
{
    Camera_updateRotation(camera, input->rotation_dx, -input->rotation_dy);
    Camera_updatePosition(camera, input->move_dx, input->move_dy, input->move_dz);
    Camera_updateVectors(camera);
    Camera_updateMatrix(camera);
    Camera_updateFrustum(camera);
}

void Camera_updateFrustum(Camera *camera)
{
    // Near plane
    camera->frustum[0].n[0] = camera->front[0];
    camera->frustum[0].n[1] = camera->front[1];
    camera->frustum[0].n[2] = camera->front[2];
    Vec3_scale(
            camera->front,
            camera->near,
            camera->frustum[0].p
    );
    Vec3_add(
            camera->position,
            camera->frustum[0].p,
            camera->frustum[0].p
    );

    // Far plane
    camera->frustum[1].n[0] = -camera->front[0];
    camera->frustum[1].n[1] = -camera->front[1];
    camera->frustum[1].n[2] = -camera->front[2];
    Vec3_scale(
            camera->front,
            camera->far,
            camera->frustum[1].p
    );
    Vec3_add(
            camera->position,
            camera->frustum[1].p,
            camera->frustum[1].p
    );

    // Used to store vector in plane
    Vec3 a;

    // Top plane
    Vec3_scale(
            camera->up,
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
            camera->position,
            a
    );

    Vec3_mul(
            a,
            camera->right,
            camera->frustum[2].n
    );
    Vec3_normalize(
            camera->frustum[2].n,
            camera->frustum[2].n
    );

    // Bottom plane
    Vec3_scale(
            camera->up,
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
            camera->position,
            a
    );

    Vec3_mul(
            camera->right,
            a,
            camera->frustum[3].n
    );
    Vec3_normalize(
            camera->frustum[3].n,
            camera->frustum[3].n
    );

    // Left plane
    Vec3_scale(
            camera->right,
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
            camera->position,
            a
    );

    Vec3_mul(
            a,
            camera->up,
            camera->frustum[4].n
    );
    Vec3_normalize(
            camera->frustum[4].n,
            camera->frustum[4].n
    );

    // Right plane
    Vec3_scale(
            camera->right,
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
            camera->position,
            a
    );

    Vec3_mul(
            camera->up,
            a,
            camera->frustum[5].n
    );
    Vec3_normalize(
            camera->frustum[5].n,
            camera->frustum[5].n
    );

}

static float planeSDF(const Plane *p, const Vec3 r)
{
    return Vec3_dot(p->n, r) - Vec3_dot(p->n, p->p);
}

int Camera_sphereInFrustum(const Camera *camera, Vec3 p, float r)
{
    for (int i = 0; i < 6; i++)
    {
        float distance = planeSDF(&camera->frustum[i], p);
        if (distance < -r)
        {
            return 0;
        }
        else if (distance < r)
        {
            return 1;
        }
    }
    return 1;
}
