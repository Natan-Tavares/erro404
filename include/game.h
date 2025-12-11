#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <quest.h>
#include <menu.h>
#include <npc.h>
#include <dialogue.h>

#define WINDOW_WIDTH 800 //Tamanho da largura da Janela do Jogo
#define WINDOW_HEIGHT 700 //Tamanho da altura da Janela do Jogo
#define MAX_LINE_LENGTH 512 //Numero maximo de caracteres numa linha de arquivo de npcs

typedef enum GameScreen {
    MENU,
    GAME,
    END,
    EXIT 

}GameScreen;

typedef struct ObjectEntity ObjectEntity;
typedef struct Popup Popup;
typedef struct ChoiceMenu ChoiceMenu;
typedef struct Dialogue Dialogue;

//Gerenciador do jogo, 8 componentes
typedef struct GameManager
{
    Player *player; //player do jogo
    GameScreen currentScreen; //Qual tela o jogo esta no momento
    Menu menu; //armazena os valores do menu do jogo
    ChoiceMenu *choiceMenu;

    int numberOfNpcEntitys; //Componente que armazena a quantidade de npcs no mapa
    int numberOfItemEntitys; //Componente que armazena a quantidade de entidades de items no mapa
    int numberOfObjectEntitys; //Componente que armazena a quantidade de objetos no mapa

    Npc *activeNpc; //Componente que armazena qual npc esta ativo 
    ObjectEntity *activeObject;
    Dialogue *activeDialogue;
    Quest activeQuest;

    Popup *activePopup;
    
    int activeQuestId;
    int activeQuestsCount;
    int interactingQuestIndex;

    int selectedOption;

}GameManager;

void UpdateGame(Player player,GameManager *gameManager);

void UpdateEndMenu(GameManager *gameManager);

void DrawEndMenu(GameManager *gameManager);

#endif
