#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>
#include <sprite.h>
#include <animation.h>

#define NUMBER_OF_OBJECTS 5

typedef struct Object
{
    int id;
    Sprite sprite;
    animation animation;

}Object;

typedef struct ObjectEntity
{

    int ObjectId;
    Vector2 position;
    Vector2 direction;
    Vector2 velocity;

    float speed;

    bool isPlayerNearby;

    bool isSolid;
    bool isPushable;
    bool isLocked;
    bool isDoor;

    int requiredItemId;
    int giftItemId;

}ObjectEntity;

typedef struct Player Player;
typedef struct GameManager GameManager;

Rectangle GetObjectHitbox(ObjectEntity self,float width,float height);

void applyVelX(ObjectEntity *self);

void applyVelY(ObjectEntity *self);

float CheckCollisionX(Rectangle hitbox,Rectangle collisionRectangle);

float CheckCollisionY(Rectangle hitbox,Rectangle collisionRectangle);

void DrawObjects(ObjectEntity *objectList,GameManager gameManager);

ObjectEntity *LoadObjects(const char *filename,int *numberOfObjects);

void FreeObjectCatalog();

void ResolvePlayerVsObjectsY(Player *player, ObjectEntity *objects,unsigned char *map, int count);
void ResolvePlayerVsObjectsX(Player *player, ObjectEntity *objects,unsigned char *map, int count);

void CheckObjectProximity(ObjectEntity *objectEntityList,Player player,GameManager *gameManager);

void InteractWithObject(ObjectEntity *objectEntityList,Player *player,GameManager *gameManager);

void UpdateObjectInteract(GameManager *gameManager,Player *player);

void DrawObjectInteract(ObjectEntity *objectEntityList,GameManager gameManager);

#endif