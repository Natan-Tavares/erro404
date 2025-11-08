
#include "camera.h"
#include <raylib.h>
#include <sprite.h>
#include <game.h>   
#include <math.h>

/*
    Função para aproximar o target da camera a um valor especifico usando interpolação
*/
void UpdateCamera2D(Camera2D *self,Vector2 target){

    self->target.x = lerp(self->target.x,target.x,0.025);
    self->target.y = lerp(self->target.y,target.y,0.025);

}
