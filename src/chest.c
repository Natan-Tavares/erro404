#include "chest.h"

#include <object.h>
#include <player.h>
#include <stdlib.h>
#include <inventory.h>
#include <utils.h>
#include <string.h>
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

bool TryToInteractWithChest(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    if(gameManager->activeObject == objectEntity) return false;
    ChestData *chestData = ((ChestData *)objectEntity->data);

    if(CheckChestProximity(objectEntity,*player) && player->canInteract && IsKeyPressed(KEY_E)){
        gameManager->activeObject = objectEntity;
        player->canInteract = false;
        return true;
    }
    return false;
}

void OpenChest(ObjectEntity *object){
    ChestData *chestData = ((ChestData *)object->data);

    chestData->isLocked = false;
    object->ObjectId++;

}

bool CheckChestInteraction(ObjectEntity *object,Player *player,int option){
    ChestData *objectData = ((ChestData *)object->data);
    player->canInteract = false;

    switch (option) {
        
        case 0:
            return CheckInventoryHasItem(player->inventory,objectData->requiredItemId,objectData->requiredItemAmount);
            break;
            
    }

    return false;

}

void UpdateChestInteraction(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    if(gameManager->activeObject != objectEntity) return;

    ChestData *objectData = ((ChestData *)objectEntity->data);

    if(IsKeyPressed(KEY_UP)) gameManager->selectedOption = (gameManager->selectedOption+1) %2; 
    if(IsKeyPressed(KEY_DOWN)) gameManager->selectedOption = (gameManager->selectedOption+1) %2;

    if(IsKeyPressed(KEY_E) && player->canInteract){

        if(CheckChestInteraction(objectEntity,player,gameManager->selectedOption)){
            RemoveItem(&(player->inventory),objectData->requiredItemId,objectData->requiredItemAmount);
            OpenChest(objectEntity);
            AddItemToInventory(&(player->inventory),objectData->giftItemId,objectData->giftItemAmount);
            PreDoneCollectItemPopup(objectData->giftItemId,&gameManager->activePopup);
        }
        
        player->canInteract = false;
        gameManager->activeObject = NULL;
        return;
    }
    UpdateBoolValue(&player->canInteract);
    gameManager->canInteract = false;

}

void UpdateChest(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){

    TryToInteractWithChest(objectEntity,player,gameManager);

    UpdateChestInteraction(objectEntity,player,gameManager);

}