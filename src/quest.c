#include "quest.h"

#include <raylib.h>
#include <dialogue.h>
#include <game.h>
#include <string.h>
#include <stdio.h>

Quest *GetQuestCatalog(){
    static Quest catalog[NUMBER_OF_QUESTS];
    static bool isInitialized = false;

    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Quest){.id=0,.requiredItemId=0,.numberOfRequiredItem=3,.status=NOT_STARTED};
        
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

void UpdateQuestChoice(GameManager *gameManager){

    DialogueStatus *dialogueStatus = &gameManager->dialogueStatus;
    if(*dialogueStatus != RESPONSE ) return; 
    Quest *quest = GetQuestById(gameManager->activeNpc->questId);
    if(quest->status == IN_PROGRESS) return;


    if(IsKeyPressed(KEY_UP)) gameManager->selectedOption = (gameManager->selectedOption+1) %2; 
    if(IsKeyPressed(KEY_DOWN)) gameManager->selectedOption = (gameManager->selectedOption+1) %2;

    if(IsKeyPressed(KEY_E)){
        if (gameManager->selectedOption == 0)
        {
            quest->status = IN_PROGRESS;
            quest->isActive = true;
        }
        gameManager->dialogueStatus = NONE;

    }

}

void DrawQuestChoice(GameManager *gameManager){
    if(gameManager->dialogueStatus != RESPONSE) return;
    
    DialogueStatus *dialogueStatus = &gameManager->dialogueStatus;

    DrawRectangle(100,600,400,120,Fade(BLACK,0.7));
    DrawText("Aceitar Missão?",120,620,20,WHITE);

    const char *options[] = {"Sim","Não"};
    for(int i = 0;i < 2;i++){
        Color color = (i == gameManager->selectedOption ) ? YELLOW : WHITE;
        DrawText(options[i],140,650 + i * 25,20,color);
    }
}