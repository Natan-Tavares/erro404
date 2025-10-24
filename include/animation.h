
#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>

typedef struct 
{
    
    Texture2D texture;
    Rectangle frame;
    int frameSpeed;
    int currentFrame;
    int framesCounter;
    int framesQuantity;
    Vector2 textureQuantityFrames;

}animation;

    //função para animar
void animate(animation *animation);

void changeAnimation(animation *animation,Texture novaTextura,int framesQuantity);

#endif
