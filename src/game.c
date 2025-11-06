#include "game.h"

#include <raylib.h>
#include <string.h>

void UpdateMenu(GameManager *game){
	
    int titleLength = strlen(game->menu.titleText);
    static float timer = 0.0f;
    float letterDelay = 0.2f;

	const char *menuOptions[] = { "START", "EXIT" };

    timer += GetFrameTime();
    if (timer >= letterDelay && game->menu.lettersShown < titleLength) {
        game->menu.lettersShown++;
        timer = 0.0f;
    }

	if (IsKeyPressed(KEY_W)) game->menu.selected--;
    if (IsKeyPressed(KEY_S)) game->menu.selected++;
    if (game->menu.selected < 0) game->menu.selected = game->menu.totalOptions - 1;
    if (game->menu.selected >= game->menu.totalOptions) game->menu.selected = 0;

    if (IsKeyPressed(KEY_ENTER)) {
        if (game->menu.selected == 0) game->currentScreen = GAME;
        else if (game->menu.selected == 1) game->currentScreen = EXIT;
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