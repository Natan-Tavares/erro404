//bibliotecas externas
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

//bibliotecas internas
#include <animation.h>
#include <player.h>
#include <sprite.h>
#include <camera.h>
#include <tilemap.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

typedef enum GameScreen { MENU, GAME, EXIT } GameScreen;

int main()
{
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Erro.404");
	SetTargetFPS(60);

	GameScreen currentScreen = MENU;

	const char *titleText = "erro.404";
    int titleLength = strlen(titleText);
    int lettersShown = 0;
    float timer = 0.0f;
    float letterDelay = 0.2f;

	const char *menuOptions[] = { "START", "EXIT" };
    int selected = 0;
    int totalOptions = 2;

    Texture textura = LoadTexture("resources/textures/Soldier.png");
	unsigned char *map = ReadMap("resources/maps/map.bin");

    Rectangle npc = (Rectangle){
        .x = 0,
        .y = 0,
        .width = 16,
        .height = 16
    };

    animation idle = {
        .first = 0,
        .last = 6,
        .current = 0,
        .speed = 0.1,
        .durationLeft = 0.1,
        .state = IDLE
    };

    Sprite sprite = (Sprite){
        .animation = idle,
        .position = (Vector2){.x = 0, .y = 0},
        .direction = (Vector2){1,0},
        .speed = 2
    };

    Camera2D camera = (Camera2D){
        .offset = (Vector2){WINDOW_WIDTH/2,WINDOW_HEIGHT/2},
        .rotation = 0.0,
        .target = sprite.position,
        .zoom = 3.0,
    };

    while (!WindowShouldClose())
    {

		if (currentScreen == MENU) {
            timer += GetFrameTime();
            if (timer >= letterDelay && lettersShown < titleLength) {
                lettersShown++;
                timer = 0.0f;
            }

			if (IsKeyPressed(KEY_W)) selected--;
            if (IsKeyPressed(KEY_S)) selected++;
            if (selected < 0) selected = totalOptions - 1;
            if (selected >= totalOptions) selected = 0;

            if (IsKeyPressed(KEY_ENTER)) {
                if (selected == 0) currentScreen = GAME;
                else if (selected == 1) currentScreen = EXIT;
            }
        }
        else if (currentScreen == GAME) {
            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MENU;

			MovePlayer(&sprite);

			applyVelX(&sprite);
			CheckTilesCollisionX(&sprite,map);        

			applyVelY(&sprite);
			CheckTilesCollisionY(&sprite,map);

			PlayerStatemachine(&sprite);

			UpdateAnimation(&(sprite.animation));

			if(GetDistanceFromSprite(&camera,sprite) > 25){
				UpdateCamera2D(&camera,sprite.position);
			}
		}	
		else if (currentScreen == EXIT) {
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

		if (currentScreen == MENU) {

			int titleFontSize = 60;
			int titleWidth = MeasureText(TextSubtext(titleText, 0, lettersShown), titleFontSize);
			int titleX = (WINDOW_WIDTH - titleWidth) / 2;
			int titleY = WINDOW_HEIGHT / 3;

			DrawText(TextSubtext(titleText, 0, lettersShown), titleX, titleY, titleFontSize, RAYWHITE);

			int optionFontSize = 30;
			int spacing = 50;

			int menuStartY = titleY + 150;
			for (int i = 0; i < totalOptions; i++) {
				int textWidth = MeasureText(menuOptions[i], optionFontSize);
				int optionX = (WINDOW_WIDTH - textWidth) / 2;
				int optionY = menuStartY + i * spacing;

				Color color = (i == selected) ? WHITE : DARKGRAY;
				DrawText(menuOptions[i], optionX, optionY, optionFontSize, color);

				if (i == selected) {
					DrawText(">", optionX - 40, optionY, optionFontSize, WHITE);
				}
			}

			const char *hint = "Use W/S para navegar | ENTER para confirmar";
			int hintFontSize = 18;
			int hintWidth = MeasureText(hint, hintFontSize);
			DrawText(hint, (WINDOW_WIDTH - hintWidth) / 2, WINDOW_HEIGHT - 100, hintFontSize, DARKGRAY);
		}
        else if (currentScreen == GAME) {

				BeginMode2D(camera);

					DrawMap(map);

					DrawPlayer(&sprite,textura);

                    DrawRectangleRec(npc,GREEN);

				EndMode2D();
		}
		EndDrawing();
    }
    
    UnloadTexture(textura);
    free(map);
    CloseWindow();
    return 0;
}
