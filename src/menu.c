#include "menu.h"
#include <raylib.h>
#include <game.h>
#include <string.h>
#include <utils.h>
#include <stdio.h>

void UpdateMenu(GameManager *gameManager){
	
    int titleLength = strlen(gameManager->menu.titleText);

	const char *menuOptions[] = { "START", "EXIT" };

	UpdateVisibleChars(gameManager->menu.titleText,&(gameManager->menu.lettersShown),0.5);

	if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) gameManager->menu.selected--;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) gameManager->menu.selected++;
    if (gameManager->menu.selected < 0) gameManager->menu.selected = gameManager->menu.totalOptions - 1;
    if (gameManager->menu.selected >= gameManager->menu.totalOptions) gameManager->menu.selected = 0;

    if (IsKeyPressed(KEY_ENTER)) {
        if (gameManager->menu.selected == 0) gameManager->currentScreen = GAME;
        else if (gameManager->menu.selected == 1) gameManager->currentScreen = EXIT;
    }

}

void DrawMenu(GameManager *game){
	int titleFontSize = 60;
	int titleWidth = MeasureText(TextSubtext(game->menu.titleText, 0, game->menu.lettersShown), titleFontSize);
	int titleX = (WINDOW_WIDTH - titleWidth) / 2;   
	int titleY = WINDOW_HEIGHT / 3;

    const char *menuOptions[] = { "START", "EXIT" };

	DrawText(TextSubtext(game->menu.titleText, 0, game->menu.lettersShown), titleX, titleY, titleFontSize, RAYWHITE);

	int optionFontSize = 30;
	int spacing = 50;

	int menuStartY = titleY + 150;
	for (int i = 0; i < game->menu.totalOptions; i++) {
		int textWidth = MeasureText(menuOptions[i], optionFontSize);
		int optionX = (WINDOW_WIDTH - textWidth) / 2;
		int optionY = menuStartY + i * spacing;

		Color color = (i == game->menu.selected) ? WHITE : DARKGRAY;
		DrawText(menuOptions[i], optionX, optionY, optionFontSize, color);

		if (i == game->menu.selected) {
			DrawText(">", optionX - 40, optionY, optionFontSize, WHITE);
		    }
	    }

	const char *hint = "Use W/S para navegar | ENTER para confirmar";
	int hintFontSize = 18;
	int hintWidth = MeasureText(hint, hintFontSize);
	DrawText(hint, (WINDOW_WIDTH - hintWidth) / 2, WINDOW_HEIGHT - 100, hintFontSize, DARKGRAY);

}