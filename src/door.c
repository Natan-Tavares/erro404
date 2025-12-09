#include <door.h>

#include <object.h>
#include <player.h>
#include <inventory.h>
#include <stdlib.h>
#include <utils.h>
#include <string.h>
#include <game.h>

void HandleDoorPosition(ObjectEntity *objectEntity,const char *value){
    sscanf(value, "%f,%f", &(objectEntity->position.x), &(objectEntity->position.y));
}

void HandleDoorRequiredItemId(ObjectEntity *objectEntity,const char *value){
    DoorData *objectData = ((DoorData *)objectEntity->data);
    objectData->requiredItemId = atoi(value);
}

void HandleDoorRequiredItemAmount(ObjectEntity *objectEntity,const char *value){
    DoorData *objectData = ((DoorData *)objectEntity->data);
    objectData->requiredItemAmount = atoi(value);
}

void HandleDoorIsLocked(ObjectEntity *objectEntity,const char *value){
    DoorData *objectData = ((DoorData *)objectEntity->data);
    if(!strcmp(value,"sim")){
        objectData->isLocked = true;
        return;
    }
    objectData->isLocked = false;
}

ObjectFieldHandler *GetDoorHandlers(){
    static ObjectFieldHandler doorHandlers[] = {
    {"position:",HandleDoorPosition},
    {"required item id:",HandleDoorRequiredItemId},
    {"required item amount:",HandleDoorRequiredItemAmount},
    {"locked:",HandleDoorIsLocked},
    {NULL, NULL},
    };

    return doorHandlers; 
}

void DoorInit(ObjectEntity *object){

    DoorData *doorData = malloc(sizeof(DoorData));

    doorData->isLocked = true;
    doorData->requiredItemId = 0;
    doorData->requiredItemAmount = 1;

    object->data = doorData;
    object->isPushable = false;

}

bool CheckDoorProximity(ObjectEntity *objectEntity,Player player){
    DoorData *doorData = ((DoorData *)objectEntity->data);
    if(!doorData->isLocked) return false;

    if(GetDistance(player.object.position,objectEntity->position) < 25){
        return true;
    }

    return false;

}

bool TryToInteractWithDoor(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    if(gameManager->activeObject == objectEntity) return false;
    DoorData *doorData = ((DoorData *)objectEntity->data);

    if(CheckDoorProximity(objectEntity,*player) && player->canInteract && IsKeyPressed(KEY_E)){
        gameManager->activeObject = objectEntity;
        player->canInteract = false;
        return true;
    }
    return false;
}

void OpenDoor(ObjectEntity *object){
    DoorData *objectData = ((DoorData *)object->data);

    object->isSolid = false;
    objectData->isLocked = false;
    object->ObjectId++;

}

bool CheckDoorInteraction(ObjectEntity *object,Player *player,int option){
    DoorData *objectData = ((DoorData *)object->data);
    player->canInteract = false;

    switch (option) {
        
        case 0:
            return CheckInventoryHasItem(player->inventory,objectData->requiredItemId,objectData->requiredItemAmount);
            break;
    }

    return false;

}

void UpdateDoorInteraction(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    if(gameManager->activeObject != objectEntity) return;

    DoorData *objectData = ((DoorData *)objectEntity->data);

    if(IsKeyPressed(KEY_UP)) gameManager->selectedOption = (gameManager->selectedOption+1) %2; 
    if(IsKeyPressed(KEY_DOWN)) gameManager->selectedOption = (gameManager->selectedOption+1) %2;

    if(IsKeyPressed(KEY_E) && player->canInteract){

        if(CheckDoorInteraction(gameManager->activeObject,player,gameManager->selectedOption)){
            RemoveItem(&(player->inventory),objectData->requiredItemId,objectData->requiredItemAmount);
            OpenDoor(objectEntity);
        }
        
        player->canInteract = false;
        gameManager->activeObject = NULL;
        return;
    }
    UpdateBoolValue(&player->canInteract);
    gameManager->canInteract = false;

}

void UpdateDoor(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){

    TryToInteractWithDoor(objectEntity,player,gameManager);

    UpdateDoorInteraction(objectEntity,player,gameManager);

}