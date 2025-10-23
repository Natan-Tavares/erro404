
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

}animation;

    //função para animar
void animate(animation *animation);

void changeAnimation(animation *animation,char *fileName,int framesQuantity);

#endif
