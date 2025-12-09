#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <raylib.h>

#define MAX_DIALOGUE_LENGTH 512 //Numero maximo de caracteres que um dialogo de npc pode ter

typedef enum DialogueStatus{
    NONE, //caso o dialogo não tenha
    CHOICE,
    RESPONSE,
    GIVE,
    GIVE_CHOICE,

}DialogueStatus;

//Dialogo,3 componentes
typedef struct Dialogue
{

    char text[MAX_DIALOGUE_LENGTH]; //Componente que armazena o texto do dialogo
    int visibleChars; // Componente que armazena a quantidade de caracteres visiveis do dialogo
    bool activate; //Componente que armazena se o dialogo esta ativo ou não

    bool canSkip;
    
}Dialogue;

typedef struct GameManager GameManager;

void StartDialogue(Dialogue *dialogue,GameManager *gameManager,DialogueStatus status);

void StopDialogue(GameManager *gameManager);

void UpdateDialogue(GameManager *gameManager);

void DrawDialogue(GameManager *gameManager);

#endif