
#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <animation.h>

typedef struct Sprite
{
    animation animation;

    Vector2 position;
    Vector2 direction;
    Vector2 velocity;
    
    float speed;
    
}Sprite;

Rectangle GetSpriteHitbox(Sprite *self);

    //Função para atualizar a posição X do sprite
void applyVelX(Sprite *self);

    //Função para atualizar a posição Y do sprite
void applyVelY(Sprite *self);

    //Função para impedir a interpolação em Y de um sprite e um retangulo 
void checkCollisionY(Sprite *self,Rectangle collisionRectangle);
			
    //Função para impedir a interpolação em X de um sprite e um retangulo
void checkCollisionX(Sprite *self,Rectangle collisionRectangle);

#endif