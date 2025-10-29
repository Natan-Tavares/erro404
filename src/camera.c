
#include "camera.h"
#include <raylib.h>
#include <sprite.h>
#include <math.h>

/*
    Função para aproximar um valor inicial de um final usando um fator
*/
float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

/*
    Função para aproximar o target da camera a um valor especifico usando interpolação
*/
void UpdateCamera2D(Camera2D *self,Vector2 target){

    self->target.x = lerp(self->target.x,target.x,0.025);
    self->target.y = lerp(self->target.y,target.y,0.025);

}

/*
    Função que faz pitagoras entre a distancia x e y do sprite e da camera para retornar
    a menor distancia entre os dois
*/
float GetDistanceFromSprite(Camera2D *self,Sprite sprite){

    Vector2 dist = (Vector2){
        .x = (float)abs((int)(self->target.x - sprite.position.x)),
        .y = (float)abs((int)(self->target.y - sprite.position.y))
    };

    return (float)sqrt((pow(dist.x,2.0)) + (pow(dist.y,2.0)));

}