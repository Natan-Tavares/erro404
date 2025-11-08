//bibliotecas externas
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//bibliotecas internas
#include <game.h>
#include <animation.h>
#include <player.h>
#include <sprite.h>
#include <camera.h>
#include <tilemap.h>
#include <npc.h>

int main()
{
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"ERR0 404");
	SetTargetFPS(60);

    Texture textura = LoadTexture("resources/textures/Soldier.png");
	unsigned char *map = ReadMap("resources/maps/map.bin");

    GameManager game = (GameManager){

        .currentScreen = MENU,
        .menu = (Menu){
            .titleText = "ERRO 404",
            .lettersShown = 0,
            .selected = 0,
            .totalOptions = 2,

        },
        .activeNpc = NULL,
        .activeDialogueindex = 0,
        .isDialogueActive = false,

    };

    Npc *npcList = LoadNpcs("resources/npcs.txt",&game.numberOfNpcs);

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

		if (game.currentScreen == MENU) {
            UpdateMenu(&game);

        }else if (game.currentScreen == GAME) {
            if (IsKeyPressed(KEY_C)) game.currentScreen = MENU;

			MovePlayer(&sprite);

			applyVelX(&sprite);
			CheckTilesCollisionX(&sprite,map);        

			applyVelY(&sprite);
			CheckTilesCollisionY(&sprite,map);

			PlayerStatemachine(&sprite);

			UpdateAnimation(&(sprite.animation));

			if(GetDistance(camera.target,sprite.position) > 25){
				UpdateCamera2D(&camera,sprite.position);
			}

            CheckAllNpcProximities(npcList,sprite,game);

            InteractWithNpc(npcList,&game);

            UpdateActiveDialogue(&game);

		}else if (game.currentScreen == EXIT) {
            break;
        }

        BeginDrawing();
            ClearBackground(BLACK);

		    if (game.currentScreen == MENU) {
                DrawMenu(&game);
    		}
            else if (game.currentScreen == GAME) {
		    	BeginMode2D(camera);

			    	DrawMap(map);

				    DrawPlayer(&sprite,textura);

                    DrawNpcs(npcList,game.numberOfNpcs);

			    EndMode2D();

                DrawActiveDialogue(&game);
		
            }

		EndDrawing();
    }

    UnloadTexture(textura);
    free(map);
    free(npcList);
    CloseWindow();
    return 0;
}
