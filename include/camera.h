
#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <sprite.h>

//Atualiza a posição da camera trazendo mais perto do alvo
void UpdateCamera2D(Camera2D *self,Vector2 target);

#endif