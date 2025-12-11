#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <raylib.h>

#define MAX_TEXT_LENGTH 512 //Numero maximo de caracteres que um dialogo de npc pode ter
#define MAX_TEXT_AMOUNT 10

typedef void (*DialogueCallback)(void *context);

typedef enum DialogueType{
    NORMAL, 
    AWAITING_ITEM,
    ASK,  
    THANKS,

}DialogueType;

//Dialogo,3 componentes
typedef struct TextLine
{

    char content[MAX_TEXT_LENGTH]; //Componente que armazena o texto do dialogo
    int visibleChars; // Componente que armazena a quantidade de caracteres visiveis do dialogo

    bool canSkip;
    
}TextLine;

typedef struct Dialogue{

    TextLine textlines[MAX_TEXT_AMOUNT];
    int activeTextLineIndex;
    int activeTextLineAmount;

    DialogueType type;

    DialogueCallback onComplete;
    void *callbackContext;

}Dialogue;

typedef struct GameManager GameManager;

Dialogue CreateBlankDialogue();

Dialogue CreateDialogueAs(DialogueType type);

void FillDialogue(Dialogue *self,const char *content);

void UpdateDialogue(Dialogue **self);

void DrawDialogue(Dialogue *self);

#endif