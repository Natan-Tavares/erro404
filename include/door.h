#ifndef DOOR_H
#define DOOR_H

#include <raylib.h>
#include <object.h>
#include <player.h>

typedef struct DoorData
{

    bool isLocked;

    int requiredItemId;
    int requiredItemAmount;

}DoorData;

ObjectFieldHandler *GetDoorHandlers();

void DoorInit(ObjectEntity *object);

void UpdateDoor(ObjectEntity *ObjectEntity,Player *player,GameManager *gameManager);

#endif