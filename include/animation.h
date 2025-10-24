
#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>

typedef enum state{

    IDLE = 0,
    WALK = 1

}state;

typedef struct animation
{
    int first;
    int last;

    int current;
    float speed;

    float durationLeft;

    state state;

}animation;


void UpdateAnimation(animation *animation);

Rectangle GetAnimationFrame(animation *animation,int numframesPerRow);

#endif
