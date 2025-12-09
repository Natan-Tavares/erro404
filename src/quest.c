#include "quest.h"

#include <raylib.h>
#include <dialogue.h>
#include <game.h>
#include <string.h>
#include <stdio.h>
#include <popup.h>
#include <inventory.h>
#include <utils.h>

Quest *GetQuestCatalog(){
    static Quest catalog[NUMBER_OF_QUESTS];
    static bool isInitialized = false;

    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Quest){.id=0,.requiredItemId=0,.numberOfRequiredItem=3,.giftItemId=1,.numberOfGiftItem=1};
        catalog[1] = (Quest){.id=1,.requiredItemId=2,.numberOfRequiredItem=1,.giftItemId=0,.numberOfGiftItem=32};
        
        isInitialized = true;
    }

    return catalog;
}

Quest *GetQuestById(int id){
    Quest *catalog = GetQuestCatalog();

    for(int i = 0;i < NUMBER_OF_QUESTS;i++){
        if(catalog[i].id == id) return &catalog[i];
    }

    return NULL;
}

void UpdateQuestChoice(Player *player,GameManager *gameManager){
    DialogueStatus *dialogueStatus = &gameManager->dialogueStatus;
    if(*dialogueStatus == NONE || *dialogueStatus == CHOICE || *dialogueStatus == GIVE) return; 
    Quest *quest = GetQuestById(gameManager->activeNpc->questId);
    if(quest->status == COMPLETED) return;

    static bool localCanInteract = false;

    if(IsKeyPressed(KEY_UP)) gameManager->selectedOption = (gameManager->selectedOption+1) %2; 
    if(IsKeyPressed(KEY_DOWN)) gameManager->selectedOption = (gameManager->selectedOption+1) %2;

    if(IsKeyPressed(KEY_E) && quest->status == NOT_STARTED && localCanInteract){
        if (gameManager->selectedOption == 0)
        {
            quest->status = IN_PROGRESS;
            quest->isActive = true;
            gameManager->activeQuestsId[gameManager->activeQuestsCount];
            gameManager->activeQuestsCount++;
        }
        gameManager->dialogueStatus = NONE;
        gameManager->canInteract = false;
        localCanInteract = false;
        return;
    }else if(IsKeyPressed(KEY_E) && quest->status == IN_PROGRESS && localCanInteract){
        if (gameManager->selectedOption == 0){
            quest->status = COMPLETED;
            quest->isActive = false;
            RemoveItem(&player->inventory,quest->requiredItemId,quest->numberOfRequiredItem);
            AddItemToInventory(&player->inventory,quest->giftItemId,quest->numberOfGiftItem);
            PreDoneCollectItemPopup(quest->giftItemId,&gameManager->activePopup);
        }
        gameManager->interactingQuestIndex = quest->id;
        gameManager->dialogueStatus = NONE;
        gameManager->canInteract = false;
        localCanInteract = false;
        return;
    }
    
    UpdateBoolValue(&localCanInteract);
    gameManager->canInteract = false;

}

void DrawQuestChoice(GameManager *gameManager){
    DialogueStatus *dialogueStatus = &gameManager->dialogueStatus;    
    if(*dialogueStatus == NONE || *dialogueStatus == CHOICE || *dialogueStatus == GIVE) return; 
    
    DrawRectangle(100,600,400,120,Fade(BLACK,0.7));
    if(*dialogueStatus == GIVE_CHOICE){
        DrawText("Entregar itens?",120,620,20,WHITE);
    }else{
        DrawText("Aceitar Missão?",120,620,20,WHITE);
    }

    const char *options[] = {"Sim","Não"};
    for(int i = 0;i < 2;i++){
        Color color = (i == gameManager->selectedOption ) ? YELLOW : WHITE;
        DrawText(options[i],140,650 + i * 25,20,color);
    }
}