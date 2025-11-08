#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#include <npc.h>
#include <stdio.h>

#define WINDOW_WIDTH 1200 //Tamanho da largura da Janela do Jogo
#define WINDOW_HEIGHT 900 //Tamanho da altura da Janela do Jogo

typedef enum GameScreen {
    MENU,
    GAME,
    EXIT 

}GameScreen;

//Menu, 4 componentes
typedef struct 
{

    char *titleText; // Componente que armazena o titulo do jogo
    int lettersShown; //Componente que armazena a quantidade de caractered do titulo que estão sendo mostradas
    int totalOptions; //Componente que armazena a qunatidade de opções do menu
    int selected; //Componente que armazena qual esta selecionada

}Menu;

//Gerenciador do jogo, 8 componentes
typedef struct GameManager
{
    GameScreen currentScreen; //Qual tela o jogo esta no momento
    Menu menu; //armazena os valores do menu do jogo

    int numberOfNpcs; //Componente que armazena a quantidade de npcs no mapa
    Npc *activeNpc; //Componente que armazena qual npc esta ativo 

    int activeDialogueindex; //Componente que armazena qual dialogo do npc ativo esta ativo
    bool isDialogueActive; //Componente que armazena se o dialogo esta ativo

    bool justPressedInteract; //Componente que armazena se o play acabou de pressionar para interagir

}GameManager;


    //Função para fazer interpolação linear de dois valores
float lerp(float start, float end, float t);

    //Função para pegar a menor distancia entre dois vetores
float GetDistance(Vector2 a,Vector2 b);

    //Função para atualizar os caracteres visivies de uma string
void UpdateVisibleChars(char *text,int *visibleChars,float lettersDelay);

    //Função para checar se um arquivo é valido
bool CheckFile(FILE *file);

    //Função para checar se o player acabou de interagir com algo
bool CheckJustInteract(GameManager *self);

    //Função para atualizar o menu do jogo
void UpdateMenu(GameManager *gameManager);

    //Função para desenhar o menu do jogo
void DrawMenu(GameManager *game);

#endif