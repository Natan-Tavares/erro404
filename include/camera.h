
#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <sprite.h>

void UpdateCamera2D(Camera2D *self,Vector2 target);

float GetDistanceFromSprite(Camera2D *self,Sprite sprite);

#endif