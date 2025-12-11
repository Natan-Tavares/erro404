#include <door.h>

#include <object.h>
#include <player.h>
#include <inventory.h>
#include <stdlib.h>
#include <choice.h>
#include <popup.h>
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

void OpenDoor(ObjectEntity *object){
    DoorData *objectData = ((DoorData *)object->data);

    object->isSolid = false;
    objectData->isLocked = false;
    object->ObjectId++;

}

void TryToOpenDoor(void *context){
    GameManager *gameManager = (GameManager *)context;
    Player *player = gameManager->player;
    DoorData *objectData = (DoorData *)gameManager->activeObject->data;

    gameManager->choiceMenu->active = false;

    if(CheckInventoryHasItem(gameManager->player->inventory,objectData->requiredItemId,objectData->requiredItemAmount)){
        OpenDoor(gameManager->activeObject);
        RemoveItem(&(player->inventory),objectData->requiredItemId,objectData->requiredItemAmount);
        return;
    }

    PreDoneWarningPopup("A Porta nem se meche",&gameManager->activePopup);

}

void RejectOpenDoor(void *context){
    GameManager *gameManager = (GameManager *)(context);

    PreDoneWarningPopup("A Porta nem se mantem estatica",&gameManager->activePopup);

}


bool TryToInteractWithDoor(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){
    DoorData *doorData = ((DoorData *)objectEntity->data);

    ChoiceMenu *choiceMenu = gameManager->choiceMenu;

    if(CheckDoorProximity(objectEntity,*player) && IsKeyPressed(KEY_E) && !choiceMenu->active){
        choiceMenu->active = true;
        choiceMenu->justOpened = true;

        gameManager->activeObject = objectEntity;
        
        strncpy(gameManager->choiceMenu->description,"Tentar abrir o porta?",sizeof(gameManager->choiceMenu->description));
        FillCallbacks(gameManager->choiceMenu,TryToOpenDoor,gameManager,RejectOpenDoor,gameManager);
        return true;
    }
    return false;
}

void UpdateDoor(ObjectEntity *objectEntity,Player *player,GameManager *gameManager){

    TryToInteractWithDoor(objectEntity,player,gameManager);

}