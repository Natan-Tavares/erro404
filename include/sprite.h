
#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <animation.h>

typedef struct Sprite
{
    animation animation;
    Vector2 position;
    Rectangle hitBoxRectangle;
    int xSpriteDirection;
    float speed;
    
    Vector2 velocity;

}Sprite;

    //Função para atualizar a posição X do sprite
void applyVelX(Sprite *sprite);

    //Função para atualizar a posição Y do sprite
void applyVelY(Sprite *sprite);

    //Função para impedir a interpolação em Y de um sprite e um retangulo 
void checkCollisionY(Sprite *sprite,Rectangle collisionRectangle);
			
    //Função para impedir a interpolação em X de um sprite e um retangulo
void checkCollisionX(Sprite *sprite,Rectangle collisionRectangle);

#endif