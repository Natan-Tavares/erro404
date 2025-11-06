#ifndef GAME_H
#define GAME_H

#include <raylib.h>

#include <npc.h>
#include <stdio.h>

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

    int numberOfNpcs;
    Npc *activeNpc;

    int activeDialogueindex;
    bool isDialogueActive;
    float dialogueDelay;

    bool justPressedInteract;

}GameManager;

bool CheckFile(FILE *file);

void UpdateMenu(GameManager *game);

void DrawMenu(GameManager *game);

#endif