#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "transform.h"

typedef struct {
    float speed;
    float sensitivity;
    Transform *transform;
} Player;

void Player_move(Player *player, UserInput *input);

#endif
