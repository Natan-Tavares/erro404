#include "game.h"

#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <string.h>
#include <inventory.h>
#include <utils.h>

void EndGame(GameManager *gameManager){
    gameManager->currentScreen = END;
}

void UpdateGame(Player player,GameManager *gameManager){
    if(CheckInventoryHasItem(player.inventory,3,1)&&CheckInventoryHasItem(player.inventory,4,1)&&CheckInventoryHasItem(player.inventory,5,1)){
        EndGame(gameManager);
    }
};

void UpdateEndMenu(GameManager *gameManager){
	
    int titleLength = strlen("Fim de jogo");

	const char *menuOptions[] = {"EXIT" };

	UpdateVisibleChars("Fim de jogo\nMeus Parabens",&(gameManager->menu.lettersShown),0.2);

    if (IsKeyPressed(KEY_ENTER)) {
        gameManager->currentScreen = EXIT;
    }

}

void DrawEndMenu(GameManager *gameManager){
	int titleFontSize = 60;
	int titleWidth = MeasureText(TextSubtext("Fim de jogo", 0, gameManager->menu.lettersShown), titleFontSize);
	int titleX = (WINDOW_WIDTH - titleWidth) / 2;   
	int titleY = WINDOW_HEIGHT / 3;

	DrawText(TextSubtext("Fim de jogo", 0, gameManager->menu.lettersShown), titleX, titleY, titleFontSize, RAYWHITE);

	int optionFontSize = 30;
	int spacing = 50;

	int menuStartY = titleY + 150;
	int textWidth = MeasureText("EXIT", optionFontSize);
	int optionX = (WINDOW_WIDTH - textWidth) / 2;
	int optionY = menuStartY * spacing;

    DrawText("EXIT", optionX, optionY, optionFontSize, YELLOW);

	DrawText(">", optionX - 40, optionY, optionFontSize, WHITE);

	const char *hint = "Use W/S para navegar | ENTER para confirmar";
	int hintFontSize = 18;
	int hintWidth = MeasureText(hint, hintFontSize);
	DrawText(hint, (WINDOW_WIDTH - hintWidth) / 2, WINDOW_HEIGHT - 100, hintFontSize, DARKGRAY);

}