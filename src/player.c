#include "player.h"

static void Player_updateRotation(Player *player, float dx, float dy)
{
    player->transform->yaw += dx * player->sensitivity;

    player->transform->pitch -= dy * player->sensitivity;
    if (player->transform->pitch > 85.0f)
    {
        player->transform->pitch = 85.0f;
    }
    if (player->transform->pitch < -85.0f)
    {
        player->transform->pitch = -85.0f;
    }
}

static void Player_updatePosition(Player *player, float dx, float dy, float dz)
{
    Vec3 movement_x;
    Vec3_scale(player->transform->right, dx, movement_x);

    Vec3 movement_y;
    Vec3_scale(player->transform->up, dy, movement_y);

    Vec3 movement_z;
    Vec3_scale(player->transform->forward, dz, movement_z);

    Vec3 movement;
    Vec3_add(movement_x, movement_y, movement);
    Vec3_add(movement_z, movement, movement);
    Vec3_normalize(movement, movement);
    Vec3_scale(movement, player->speed, movement);

    Vec3_add(
            player->transform->position,
            movement,
            player->transform->position
    );
}

void Player_move(Player *player, UserInput *input)
{
    Player_updateRotation(
            player,
            input->rotation_dx,
            input->rotation_dy
    );

    Player_updatePosition(
            player,
            input->move_dx,
            input->move_dy,
            input->move_dz
    );

    Transform_updateVectors(player->transform);
}
