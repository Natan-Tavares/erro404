#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>

typedef struct Object
{

    Vector2 position;
    Vector2 direction;
    Vector2 velocity;

    float speed;

}Object;

Rectangle GetObjectHitbox(Object self,float width,float height);

void applyVelX(Object *self);

void applyVelY(Object *self);

float CheckCollisionX(Rectangle hitbox,Rectangle collisionRectangle);

float CheckCollisionY(Rectangle hitbox,Rectangle collisionRectangle);

#endif