#ifndef CHEST
#define CHEST

#include <raylib.h>
#include <object.h>
#include <player.h>

typedef struct ChestData
{

    bool isLocked;

    int requiredItemId;
    int requiredItemAmount;
    int giftItemId;
    int giftItemAmount;

}ChestData;

ObjectFieldHandler *GetChestHandlers();

void ChestInit(ObjectEntity *object);

void UpdateChest(ObjectEntity *ObjectEntity,Player *player,GameManager *gameManager);

#endif