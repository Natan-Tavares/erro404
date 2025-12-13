#include "quest.h"

#include <raylib.h>
#include <dialogue.h>
#include <game.h>
#include <string.h>
#include <npc.h>
#include <choice.h>
#include <stdio.h>
#include <popup.h>
#include <inventory.h>
#include <utils.h>

Quest *GetQuestCatalog(){
    static Quest catalog[NUMBER_OF_QUESTS];
    static bool isInitialized = false;

    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Quest){.id=0,.requiredItemId=0,.requiredItemAmount=3,.giftItemId=1,.giftItemAmount=1};
        catalog[1] = (Quest){.id=1,.requiredItemId=2,.requiredItemAmount=1,.giftItemId=0,.giftItemAmount=32};
        
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

void StartQuestChoice(void *context){
    GameManager *game = (GameManager *)context;

    Dialogue *dialogue = game->activeDialogue;

    switch (dialogue->type)
    {
        case AWAITING_ITEM:
            game->choiceMenu->active = true;

            strncpy(game->choiceMenu->description,"Entregar Itens?",sizeof(game->choiceMenu->description));
            FillCallbacks(game->choiceMenu,TryToGiveQuestItemsCallback,game,RejectCallback,game);
            break;

        case ASK:
            game->choiceMenu->active = true;

            strncpy(game->choiceMenu->description,"Aceitar Missão?",sizeof(game->choiceMenu->description));      
            FillCallbacks(game->choiceMenu,AcceptQuestCallback,game,RejectCallback,game);
            break;

        default:
            break;
            
    }

}

void AcceptQuestCallback(void *context){
    GameManager *gameManager = (GameManager *)context;

    Quest *quest = GetQuestById(gameManager->activeQuestId);
    quest->status = IN_PROGRESS;

    PreDoneAcceptQuestPopUp("Missão Aceita",&gameManager->activePopup);
    gameManager->player->canTalk = false;

}

void GiveQuestItems(GameManager *gameManager){
    Quest *quest = GetQuestById(gameManager->activeQuestId);

    quest->status = COMPLETED;
    RemoveItem(&gameManager->player->inventory,quest->requiredItemId,quest->requiredItemAmount);
    AddItemToInventory(&gameManager->player->inventory,quest->giftItemId,quest->giftItemAmount);
    PreDoneCollectItemPopup(quest->giftItemId,&gameManager->activePopup);
}

void TryToGiveQuestItemsCallback(void *context){
    GameManager *gameManager = (GameManager *)context;
    gameManager->player->canTalk = false;

    Quest *quest = GetQuestById(gameManager->activeQuestId);

    if(CheckInventoryHasItem(gameManager->player->inventory,quest->requiredItemId,quest->requiredItemAmount)){
        GiveQuestItems(gameManager);

        gameManager->activeDialogue = &gameManager->activeNpc->thanksDialogue;
        DefineCallbacks(gameManager->activeDialogue,StartQuestChoice,gameManager,DrawNpcDialogue,gameManager);
        return;
    }

    PreDoneWarningPopup("Não tem os itens necessarios",&gameManager->activePopup);
    
}
 
void RejectCallback(void *context){

}
