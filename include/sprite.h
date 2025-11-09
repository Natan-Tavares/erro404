#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <animation.h>

//sprite, 2 componentes
typedef struct Sprite
{
    Texture2D texture;
    animation animation; // componente dos valores da animação

}Sprite;

#endif