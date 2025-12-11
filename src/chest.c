#include "chest.h"

#include <object.h>
#include <player.h>
#include <stdlib.h>
#include <inventory.h>
#include <utils.h>
#include <string.h>
#include <choice.h>
#include <popup.h>
#include <game.h>

void HandleChestPosition(ObjectEntity *objectEntity,const char *value){
    sscanf(value, "%f,%f", &(objectEntity->position.x), &(objectEntity->position.y));
}

void HandleChestRequiredItemId(ObjectEntity *objectEntity,const char *value){
    ChestData *objectData = ((ChestData *)objectEntity->data);
    objectData->requiredItemId = atoi(value);
}

void HandleChestRequiredItemAmount(ObjectEntity *objectEntity,const char *value){
    ChestData *objectData = ((ChestData *)objectEntity->data);
    objectData->requiredItemAmount = atoi(value);
}

void HandleChestGiftItemId(ObjectEntity *objectEntity,const char *value){
    ChestData *objectData = ((ChestData *)objectEntity->data);
    objectData->giftItemId = atoi(value);
}

void HandleChestGiftItemAmount(ObjectEntity *objectEntity,const char *value){
    ChestData *objectData = ((ChestData *)objectEntity->data);
    objectData->giftItemAmount = atoi(value);
}

void HandleChestIsLocked(ObjectEntity *objectEntity,const char *value){
    ChestData *objectData = ((ChestData *)objectEntity->data);
    if(!strcmp(value,"sim")){
        objectData->isLocked = true;
        return;
    }
    objectData->isLocked = false;
}

ObjectFieldHandler *GetChestHandlers(){
    static ObjectFieldHandler chestHandlers[] = {
    {"position:",HandleChestPosition},
    {"required item id:",HandleChestRequiredItemId},
    {"required item amount:",HandleChestRequiredItemAmount},
    {"gift item id:",HandleChestGiftItemId},
    {"gift item amount:",HandleChestGiftItemAmount},
    {"locked:",HandleChestIsLocked},
    {NULL, NULL},
    };

    return chestHandlers; 
}

void ChestInit(ObjectEntity *object){

    ChestData *chestData = malloc(sizeof(ChestData));

    chestData->isLocked = true;
    chestData->requiredItemId = 0;
    chestData->requiredItemAmount = 1;
    chestData->giftItemAmount = 0;

    object->data = chestData;
}

bool CheckChestProximity(ObjectEntity *objectEntity,Player player){
    ChestData *chestData = ((ChestData *)objectEntity->data);
    if(!chestData->isLocked) return false;

    if(GetDistance(player.object.position,objectEntity->position) < 25){
        return true;
    }

    return false;

}

void OpenChest(ObjectEntity *object){
    ChestData *chestData = ((ChestData *)object->data);

    chestData->isLocked = false;
    object->ObjectId++;

}

void TryToOpenChest(void *context){
    GameManager *gameManager = (GameManager *)context;
    Player *player = gameManager->player;
    ChestData *objectData = (ChestData *)gameManager->activeObject->data;

    gameManager->choiceMenu->active = false;

    if(CheckInventoryHasItem(gameManager->player->inventory,objectData->requiredItemId,objectData->requiredItemAmount)){
        OpenChest(gameManager->activeObject);
        RemoveItem(&(player->inventory),objectData->requiredItemId,objectData->requiredItemAmount);
        AddItemToInventory(&(player->inventory),objectData->giftItemId,objectData->giftItemAmount);
        PreDoneCollectItemPopup(objectData->giftItemId,&gameManager->activePopup);
        return;
    }

    PreDoneWarningPopup("A tranca não sai do lugar",&gameManager->activePopup);

}

void RejectOpenChest(void *context){
    GameManager *gameManager = (GameManager *)context;

    PreDoneWarningPopup("O Bau olha fundo nos seus olhos",&gameManager->activePopup);
    gameManager->choiceMenu->active = false;

}

bool TryToInteractWithChest(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    ChestData *chestData = ((ChestData *)objectEntity->data);

    if(CheckChestProximity(objectEntity,*player) && IsKeyPressed(KEY_E) && !gameManager->choiceMenu->active){
        gameManager->choiceMenu->active = true;
        gameManager->choiceMenu->justOpened = true;
        gameManager->activeObject = objectEntity;
        player->canInteract = false;
        strncpy(gameManager->choiceMenu->description,"Tentar abrir o baú?",sizeof(gameManager->choiceMenu->description));
        FillCallbacks(gameManager->choiceMenu,TryToOpenChest,gameManager,RejectOpenChest,gameManager);
        return true;
    }
    return false;
}

void UpdateChest(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){

    TryToInteractWithChest(objectEntity,player,gameManager);

}