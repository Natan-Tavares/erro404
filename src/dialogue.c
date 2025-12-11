#include "dialogue.h"
#include <game.h>
#include <raylib.h>
#include <string.h>
#include <utils.h>
#include <npc.h>

Dialogue CreateBlankDialogue(){
    Dialogue dialogue = {0};
    return dialogue;
}

Dialogue CreateDialogueAs(DialogueType type){
    Dialogue dialogue = {0};
    
    dialogue.type = type;

    return dialogue;
}

void FillDialogue(Dialogue *self,const char *content){
    if(self->activeTextLineAmount >= MAX_TEXT_AMOUNT) return;
    
    TextLine textLine = (TextLine){.canSkip=false,.visibleChars=0};

    strncpy(textLine.content, content, MAX_TEXT_LENGTH - 1);
    textLine.content[MAX_TEXT_LENGTH - 1] = '\0';
    replaceEscapedNewlines(textLine.content);

    self->textlines[self->activeTextLineAmount] = textLine;
    self->activeTextLineAmount++;
}

void SkipTextLine(TextLine *textLine){
    textLine->visibleChars = strlen(textLine->content);
}

bool TryToSkipTextLine(TextLine *textLine){
    if(IsKeyPressed(KEY_E) && textLine->canSkip){
        SkipTextLine(textLine);
    
        textLine->canSkip = false;
        return true;
    }

    textLine->canSkip = true;
    return false;
}

bool ProcessDialogue(Dialogue *self){

    TextLine *activeTextLine = &self->textlines[self->activeTextLineIndex];

    if(self->activeTextLineIndex >= self->activeTextLineAmount || activeTextLine->content[0] == '\0'){
        self->activeTextLineIndex = 0;
        return true;
    }

    int textLen = strlen(activeTextLine->content);

    if(activeTextLine->visibleChars < textLen){
        UpdateVisibleChars(activeTextLine->content,&activeTextLine->visibleChars,0.05);
        TryToSkipTextLine(activeTextLine);

        return false;
    }else if(IsKeyPressed(KEY_E)){
        activeTextLine->visibleChars = 0;
        self->activeTextLineIndex++;
        return false;
    }

    return false;

}

void UpdateDialogue(Dialogue **self){
    if(!(*self)) return;

    if(ProcessDialogue(*self)){
        if((*self)->onComplete){
            //ele roda StartQuestChoice com o gameManager como contexto em todos so dialogos, Não colocar a função diretamente para evitar acoplamento
            (*self)->onComplete((*self)->callbackContext);
        }
        
        (*self) = NULL;
    
    }
    
}

void DrawDialogue(Dialogue *self){
    if(!self) return;

    TextLine activeTextLine = self->textlines[self->activeTextLineIndex]; 

    int screenWidth = WINDOW_WIDTH;
    int screenHeight = WINDOW_HEIGHT;

    int boxWidth = screenWidth - 200;
    int boxHeight = 200;
    int boxX = 100;
    int boxY = screenHeight - boxHeight - 50;

    DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(BLACK, 0.7f));
    DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(WHITE, 0.3f));

    const char *visibleText = TextSubtext(activeTextLine.content, 0, activeTextLine.visibleChars);
    DrawTextEx(GetFontDefault(), visibleText, 
            (Vector2){boxX + 20, boxY + 50}, 20, 2, WHITE);


    int textLen = strlen(activeTextLine.content);
    if (activeTextLine.visibleChars >= textLen) {
        DrawText("Pressione [E] para continuar", boxX + boxWidth - 350, boxY + boxHeight - 30, 18, GRAY);
    }
}
