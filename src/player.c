#include <raylib.h>
#include <sprite.h>
#include "player.h"

    //Função para atualizar a velocidade do player
void MovePlayer(Sprite *player){

    int xDirection = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
    int yDirection = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);

    player->xSpriteDirection = xDirection;

    player->velocity.x = (float)xDirection * player->speed;
    player->velocity.y = (float)yDirection * player->speed;

}

void SetPlayerAnimation(Sprite *player,States state){

    switch (state)
    {   
    case IDLE:
        player->animation.frame.y = (int)IDLE * player->animation.texture.height/7;
        player->animation.framesQuantity = 6;

        break;
    
    case WALK:
        player->animation.frame.y = (int)WALK * player->animation.texture.height/7;
        player->animation.framesQuantity = 8;
        break;
    }

}

int GetPlayerDirection(Sprite *player){

    if (player->xSpriteDirection != 0) return player->xSpriteDirection;
    
    return 1;

}

void PlayerStatemachine(Sprite *player){

    States state;

    if(player->xSpriteDirection == 0){
        state = IDLE;
    }else{
        state = WALK;
    }

    SetPlayerAnimation(player,state);

}
