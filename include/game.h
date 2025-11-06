#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#include <npc.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

typedef enum GameScreen {
    MENU,
    GAME,
    EXIT 

}GameScreen;

typedef struct 
{

    const char *titleText;
    int lettersShown;
    int totalOptions;
    int selected;

}Menu;

typedef struct GameManager
{
    GameScreen currentScreen;
    Menu menu;

    Npc *activeNpc;
    int activeDialogueindex;
    bool isDialogueActive;

}GameManager;

void UpdateMenu(GameManager *game);

void DrawMenu(GameManager *game);




#endif