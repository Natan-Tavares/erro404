
#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <animation.h>

//sprite, 5 componentes
typedef struct Sprite
{
    animation animation; // componente dos valores da animação

    Vector2 position; // componente da posição x e y do sprite
    Vector2 direction; // componente da direção x e y do sprite
    Vector2 velocity; // componente da direção multiplicada pela speed
    
    float speed; // componente que armazena a velocidade de movimento
    
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