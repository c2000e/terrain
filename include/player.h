#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "transform.h"
#include "types.h"

typedef struct {
    f32 speed;
    f32 sensitivity;
    Transform *transform;
} Player;

void Player_move(Player *player, UserInput *input);

#endif
