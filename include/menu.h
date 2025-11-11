#ifndef MENU_H
#define MENU_H

#include <raylib.h>

typedef struct GameManager GameManager;

//Menu, 4 componentes
typedef struct Menu
{

    char *titleText; // Componente que armazena o titulo do jogo
    int lettersShown; //Componente que armazena a quantidade de caractered do titulo que estão sendo mostradas
    int totalOptions; //Componente que armazena a qunatidade de opções do menu
    int selected; //Componente que armazena qual esta selecionada

}Menu;

    //Função para atualizar o menu do jogo
void UpdateMenu(GameManager *gameManager);

    //Função para desenhar o menu do jogo
void DrawMenu(GameManager *game);

#endif