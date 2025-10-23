#include <raylib.h>
#include "sprite.h"

    //Função para atualizar a posição X do sprite
void applyVelX(Sprite *sprite){
    sprite->position.x += sprite->velocity.x;
}

    //Função para atualizar a posição Y do sprite
void applyVelY(Sprite *sprite){
    sprite->position.y += sprite->velocity.y;
}

    //Função para impedir a interpolação em Y de um sprite e um retangulo 
void checkCollisionY(Sprite *sprite,Rectangle collisionRectangle){

    sprite->hitBoxRectangle = (Rectangle){
        sprite->position.x,
        sprite->position.y,
        sprite->animation.frame.width,
        sprite->animation.frame.height,
    };

    if (CheckCollisionRecs(sprite->hitBoxRectangle, collisionRectangle)) {
            
            float overlapUp  = (sprite->hitBoxRectangle.y + sprite->hitBoxRectangle.height) - collisionRectangle.y;
            float overlapDown = (collisionRectangle.y + collisionRectangle.height) - sprite->hitBoxRectangle.y;

            if (overlapUp < overlapDown) {
                sprite->position.y -= overlapUp;
            } else {
                sprite->position.y += overlapDown;
            }
            sprite->hitBoxRectangle.y = sprite->position.y;
        }
    }

    //Função para impedir a interpolação em X de um sprite e um retangulo
void checkCollisionX(Sprite *sprite,Rectangle collisionRectangle){

        sprite->hitBoxRectangle = (Rectangle){
        sprite->position.x,
        sprite->position.y,
        sprite->animation.frame.width,
        sprite->animation.frame.height,
    };

    if (CheckCollisionRecs(sprite->hitBoxRectangle, collisionRectangle)) {
            
            float overlapLeft  = (sprite->hitBoxRectangle.x + sprite->hitBoxRectangle.width) - collisionRectangle.x;
            float overlapRight = (collisionRectangle.x + collisionRectangle.width) - sprite->hitBoxRectangle.x;

            if (overlapLeft < overlapRight) {
                sprite->position.x -= overlapLeft;
            } else {
                sprite->position.x += overlapRight;
            }

            sprite->hitBoxRectangle.x = sprite->position.x;

        }
    }