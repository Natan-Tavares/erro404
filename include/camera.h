
#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <sprite.h>

//Atualiza a posição da camera trazendo mais perto do alvo
void UpdateCamera2D(Camera2D *self,Vector2 target);

//Diz a menor distancia do sprite ate a camera
float GetDistanceFromSprite(Camera2D *self,Sprite sprite);

#endif