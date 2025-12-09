#include "dialogue.h"
#include <game.h>
#include <raylib.h>
#include <string.h>
#include <utils.h>
#include <npc.h>

void StartDialogue(Dialogue *dialogues,GameManager *gameManager,DialogueStatus status){
    if(!dialogues || dialogues->text[0] == '\0') return; //evita (caso um npc não tenha dialogos) aparecer vazio o espaço do dialogo;

    gameManager->activeDialogues = dialogues;
    gameManager->dialogueStatus = status;
    gameManager->selectedOption = 0;
}

void SkipDialogue(Dialogue *dialogue){
    dialogue->visibleChars = strlen(dialogue->text);
}

bool TryToSkipDialogue(Dialogue *dialogue){
    if(IsKeyPressed(KEY_E) && dialogue->canSkip){
        SkipDialogue(dialogue);
        dialogue->canSkip = false;
        return true;
    }
    dialogue->canSkip = true;
    return false;
}

bool PassthroughAllDialogues(Dialogue *dialogues,GameManager *gameManager){

    Dialogue *activeDialogue = &dialogues[gameManager->activeDialogueIndex];

    if(activeDialogue->text[0] == '\0'){
        gameManager->activeDialogues = NULL;             
        return true;
    }

    int textLen = strlen(activeDialogue->text);

    if(activeDialogue->visibleChars < textLen){
        UpdateVisibleChars(activeDialogue->text,&activeDialogue->visibleChars,0.05);

        TryToSkipDialogue(activeDialogue);

        return false;
    }else if(IsKeyPressed(KEY_E) && gameManager->canInteract){
        gameManager->activeDialogueIndex++;
        activeDialogue->visibleChars = 0;
        return false;
    }

    return false;
}

void UpdateDialogue(GameManager *gameManager){
    Dialogue *dialogues = gameManager->activeDialogues;
    if(!dialogues) return;

    if(PassthroughAllDialogues(dialogues,gameManager)){
        DialogueStatus *status = &gameManager->dialogueStatus;

        switch (*status)
        {
            case NONE:
                break;
        
            case CHOICE:
                *status = RESPONSE;
                break;

            case GIVE:
                *status = GIVE_CHOICE;
                break;

            default:
                break;
        }

        gameManager->activeDialogueIndex = 0;
        return;
    }
    
}

void DrawDialogue(GameManager *gameManager) {
    Dialogue *dialogues = gameManager->activeDialogues;
    if (!dialogues) return;

    int screenWidth = WINDOW_WIDTH;
    int screenHeight = WINDOW_HEIGHT;

    int boxWidth = screenWidth - 200;
    int boxHeight = 200;
    int boxX = 100;
    int boxY = screenHeight - boxHeight - 50;

    DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(BLACK, 0.7f));
    DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(WHITE, 0.3f));

    Npc *npc = GetNpcById(gameManager->activeNpc->id);
 
    if (npc->name && strlen(npc->name) > 0) {
        DrawText(npc->name, boxX + 20, boxY + 10, 24, YELLOW);
    }

    const char *visibleText = TextSubtext(dialogues[gameManager->activeDialogueIndex].text, 0, dialogues[gameManager->activeDialogueIndex].visibleChars);
    DrawTextEx(GetFontDefault(), visibleText, 
            (Vector2){boxX + 20, boxY + 50}, 20, 2, WHITE);


    int textLen = strlen(dialogues[gameManager->activeDialogueIndex].text);
    if (dialogues[gameManager->activeDialogueIndex].visibleChars >= textLen) {
        DrawText("Pressione [E] para continuar", boxX + boxWidth - 350, boxY + boxHeight - 30, 18, GRAY);
    }
}
