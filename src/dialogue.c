#include "dialogue.h"
#include <game.h>
#include <raylib.h>
#include <string.h>
#include <utils.h>
#include <npc.h>

void StartDialogue(Dialogue *dialogue,GameManager *gameManager,DialogueStatus status){
    if(strlen(dialogue->text) == 0) return; //evita caso um npc não tenha dialogos, de aparecer vazio o espaço do dialogo

    gameManager->activeDialogue = dialogue;

    gameManager->dialogueStatus = status;
    gameManager->selectedOption = 0;

    
}

void UpdateDialogue(GameManager *gameManager){
    Dialogue *dialogue = gameManager->activeDialogue;
    if(!dialogue) return;

    static bool localCanInteract = false;

    int textLen = strlen(dialogue->text);

    if(dialogue->visibleChars < textLen){
        UpdateVisibleChars(dialogue->text,&dialogue->visibleChars,0.05);

        if(IsKeyPressed(KEY_E) && localCanInteract){
            dialogue->visibleChars = textLen;   
            localCanInteract = false;
            return;
        }

    }else if(IsKeyPressed(KEY_E) && localCanInteract){
        DialogueStatus *status = &gameManager->dialogueStatus;

        gameManager->activeDialogue = NULL;

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
        dialogue->visibleChars = 0;
        localCanInteract = false;
        return;
    }
    UpdateBoolValue(&localCanInteract);

}

void DrawDialogue(GameManager *gameManager) {
    Dialogue *dialogue = gameManager->activeDialogue;
    if (!dialogue) return;

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

    const char *visibleText = TextSubtext(dialogue->text, 0, dialogue->visibleChars);
    DrawTextEx(GetFontDefault(), visibleText, 
            (Vector2){boxX + 20, boxY + 50}, 20, 2, WHITE);


    int textLen = strlen(dialogue->text);
    if (dialogue->visibleChars >= textLen) {
        DrawText("Pressione [E] para continuar", boxX + boxWidth - 350, boxY + boxHeight - 30, 18, GRAY);
    }
}
