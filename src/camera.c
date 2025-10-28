
#include "camera.h"
#include <raylib.h>
#include <sprite.h>
#include <math.h>

float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}


void UpdateCamera2D(Camera2D *self,Vector2 target){

    self->target.x = lerp(self->target.x,target.x,0.025);
    self->target.y = lerp(self->target.y,target.y,0.025);

}

float GetDistanceFromSprite(Camera2D *self,Sprite sprite){

    Vector2 dist = (Vector2){
        .x = (float)abs((int)(self->target.x - sprite.position.x)),
        .y = (float)abs((int)(self->target.y - sprite.position.y))
    };

    return (float)sqrt((pow(dist.x,2.0)) + (pow(dist.y,2.0)));

}