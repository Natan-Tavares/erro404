
#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>

typedef enum {
    IDLE = 0, 
    WALK = 1,
} States;

void MovePlayer(Sprite *player);

void SetPlayerAnimation(Sprite *player,States state);

int GetPlayerDirection(Sprite *player);

void PlayerStatemachine(Sprite *player);

#endif