#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <npc.h>
#include <dialogue.h>
#include <menu.h>

#define WINDOW_WIDTH 1200 //Tamanho da largura da Janela do Jogo
#define WINDOW_HEIGHT 900 //Tamanho da altura da Janela do Jogo

typedef enum GameScreen {
    MENU,
    GAME,
    EXIT 

}GameScreen;

//Gerenciador do jogo, 8 componentes
typedef struct GameManager
{
    GameScreen currentScreen; //Qual tela o jogo esta no momento
    Menu menu; //armazena os valores do menu do jogo

    int numberOfNpcEntitys; //Componente que armazena a quantidade de npcs no mapa
    int numberOfItemEntitys; //Componente que armazena a quantidade de entidades de items no mapa

    Npc *activeNpc; //Componente que armazena qual npc esta ativo 
    Dialogue *activeDialogue;
    DialogueStatus dialogueStatus;
    bool dialogueInputConsumed;

    int selectedOption;

    int activeDialogueindex; //Componente que armazena qual dialogo do npc ativo esta ativo
    bool isDialogueActive; //Componente que armazena se o dialogo esta ativo

    bool justInteract; //Componente que armazena se o play acabou de pressionar para interagir

}GameManager;

#endif