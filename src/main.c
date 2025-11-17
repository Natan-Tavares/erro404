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
#include <item.h>
#include <menu.h>
#include <dialogue.h>
#include <quest.h>
#include <utils.h>

int main()
{
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"ERR0 404");
	SetTargetFPS(60);

	unsigned char *map = ReadMap("resources/maps/map.bin");

    GameManager game = (GameManager){

        .currentScreen = MENU,
        .menu = (Menu){
            .titleText = "ERRO 404",
            .lettersShown = 0,
            .selected = 0,
            .totalOptions = 2,

        },
        .activeQuestsCount = 0,
        .activeNpc = NULL,
        .canInteract = true,

    };

    NpcEntity *npcEntityList = LoadNpcs("resources/npcs.txt",&game.numberOfNpcEntitys);
    ItemEntity *ItemEntitylist = LoadItems("resources/items.txt",&game.numberOfItemEntitys);
    ObjectEntity *objectEntityList = LoadObjects("resources/objects.txt",&game.numberOfObjectEntitys);

    animation idle = {
        .first = 0,
        .last = 6,
        .current = 0,
        .speed = 0.1,
        .durationLeft = 0.1,
        .state = IDLE
    };

    Player player = (Player){
        .object = (ObjectEntity){
            .position = (Vector2){0,0},
            .direction = (Vector2){1,1},
            .speed = 2.0,
        },
        .sprite = (Sprite){
            .texture = LoadTexture("resources/textures/Soldier.png"),
            .animation = idle,        
        },

    };

    Camera2D camera = (Camera2D){
        .offset = (Vector2){WINDOW_WIDTH/2,WINDOW_HEIGHT/2},
        .rotation = 0.0,
        .target = player.object.position,
        .zoom = 3.0,
    };

    ObjectEntity caixa = (ObjectEntity){
        .isPushable = true,
        .position = {300,300},
    };

    InitInventory(&player.inventory);

    while (!WindowShouldClose())
    {

		if (game.currentScreen == MENU) {
            UpdateMenu(&game);

        }else if (game.currentScreen == GAME) {
            if (IsKeyPressed(KEY_C)) game.currentScreen = MENU;
            
            MovePlayer(&player);

            applyVelX(&(player.object));
            CheckTilesCollisionX(&(player.object), map);

            ResolvePlayerVsObjectsX(&player, objectEntityList,map, game.numberOfObjectEntitys);

            applyVelY(&(player.object));
            CheckTilesCollisionY(&(player.object), map);

            ResolvePlayerVsObjectsY(&player, objectEntityList,map, game.numberOfObjectEntitys);

			PlayerStatemachine(&player);

			UpdateAnimation(&(player.sprite.animation));

            UpdateItemEntity(ItemEntitylist,game.numberOfItemEntitys,&player);

            UpdateItemsAnimation();

			if(GetDistance(camera.target,player.object.position) > 25) UpdateCamera2D(&camera,player.object.position);

            UpdateNpc(&player,npcEntityList,&game);


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

				    DrawPlayer(&player);

                    DrawNpcs(npcEntityList,game.numberOfNpcEntitys);

                    DrawItemEntityList(ItemEntitylist,game.numberOfItemEntitys);
                    
                    DrawObjects(objectEntityList,game);

			    EndMode2D();

                DrawDialogue(&game);

                DrawQuestChoice(&game);

                DrawInventory(&player.inventory,(Vector2){100,100});
            }

		EndDrawing();
    }

    UnloadTexture(player.sprite.texture);
    free(map);
    free(npcEntityList);
    free(ItemEntitylist);
    free(objectEntityList);
    FreeNpcCatalog( );
    FreeItemCatalog();
    FreeObjectCatalog();
    CloseWindow();
    return 0;
}
