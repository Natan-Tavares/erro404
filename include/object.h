#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>
#include <sprite.h>
#include <animation.h>

#define NUMBER_OF_OBJECTS 5

typedef struct Player Player;
typedef struct GameManager GameManager;

typedef struct ObjectEntity
{

    int ObjectId;
    Vector2 position;
    Vector2 direction;
    Vector2 velocity;

    float speed;

    bool isPlayerNearby;
    bool isPushable;
    bool isSolid;

    void *data;

}ObjectEntity;

typedef void (*ObjectValueHandler)(ObjectEntity *objectEntity,const char *value);

typedef struct ObjectFieldHandler
{

    const char *key;
    ObjectValueHandler handle;
}ObjectFieldHandler;

typedef struct Object
{
    int id;
    Sprite sprite;
    animation animation;

    ObjectFieldHandler *(*Handlers)();
    void (*OnUpdate)(ObjectEntity *self,Player *player,GameManager *gameManager);
    void (*OnInit)(ObjectEntity *self);

}Object;

Rectangle GetObjectHitbox(ObjectEntity self,float width,float height);

void applyVelX(ObjectEntity *self);

void applyVelY(ObjectEntity *self);

void FreeObjectCatalog();

float CheckCollisionX(Rectangle hitbox,Rectangle collisionRectangle);

float CheckCollisionY(Rectangle hitbox,Rectangle collisionRectangle);

void DrawObjects(ObjectEntity *objectList,GameManager gameManager);

ObjectEntity *LoadObjects(const char *filename,int *numberOfObjects);

void ResolvePlayerVsObjectsY(Player *player, ObjectEntity *objects,unsigned char *map, int count);
void ResolvePlayerVsObjectsX(Player *player, ObjectEntity *objects,unsigned char *map, int count);

void UpdateObjectEntitys(ObjectEntity *objectEntitys,Player *player,GameManager *gameManager);

void DrawObjectInteract(ObjectEntity *objectEntityList,GameManager gameManager);

void FreeAllObjectEntitys(ObjectEntity *objectEntitys);

#endif