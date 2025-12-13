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
#include <door.h>
#include <choice.h>
#include <camera.h>
#include <tilemap.h>
#include <npc.h>
#include <item.h>
#include <popup.h>
#include <menu.h>
#include <dialogue.h>
#include <quest.h>
#include <utils.h>

int main()
{
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"ERR0 404");
    InitAudioDevice();
	SetTargetFPS(60);

    Music music = LoadMusicStream("resources/audios/Game music.wav");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.3f);

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
    };

    ChoiceMenu choiceMenu = CreateChoiceMenu();
    game.choiceMenu = &choiceMenu;

    NpcEntity *npcEntityList = LoadNpcs("resources/npcs.txt",&game.numberOfNpcEntitys);
    ItemEntity *ItemEntitylist = LoadItems("resources/items.txt",&game.numberOfItemEntitys);
    ObjectEntity *objectEntityList = LoadObjects("resources/objects.txt",&game.numberOfObjectEntitys);

    animation idle = {
        .first = 0,
        .last = 6,
        .current = 0,
        .speed = 0.15,
        .durationLeft = 0.1,
        .state = IDLE
    };

    Player player = (Player){
        .object = (ObjectEntity){
            .position = (Vector2){96,256},
            .direction = (Vector2){1,1},
            .speed = 2.0,
        },
        .sprite = (Sprite){
            .texture = LoadTexture("resources/textures/player.png"),
            .animation = idle,        
        },
        .canInteract = true,
        .canTalk = true,

    };

    Camera2D camera = (Camera2D){
        .offset = (Vector2){WINDOW_WIDTH/2,WINDOW_HEIGHT/2},
        .rotation = 0.0,
        .target = player.object.position,
        .zoom = 4.0,
    };

    ObjectEntity caixa = (ObjectEntity){
        .isPushable = true,
        .position = {300,300},
    };

    game.player = &player;

    InitInventory(&player.inventory);

    while (!WindowShouldClose())
    {

        UpdateMusicStream(music);
        
		if (game.currentScreen == MENU) {
            UpdateMenu(&game);

        }else if (game.currentScreen == GAME) {

            if (IsKeyPressed(KEY_C)) game.currentScreen = MENU;

            UpdatePlayer(&player,objectEntityList,map,game);

			if(GetDistance(camera.target,player.object.position) > 25) UpdateCamera2D(&camera,player.object.position);

            UpdateObjectEntitys(objectEntityList,&player,&game);

            UpdateItemEntity(ItemEntitylist,&game,&player);
            UpdateItemsAnimation();

            UpdateNpc(&player,npcEntityList,&game);

            UpdateDialogue(&game.activeDialogue);

            UpdateChoiceMenu(game.choiceMenu);

            UpdatePopup(&game.activePopup);

            UpdateGame(player,&game);

            if(IsKeyPressed(KEY_E)) game.player->canInteract = false;
            else game.player->canInteract = true;

		}else if(game.currentScreen == END){
            UpdateEndMenu(&game);

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

                    DrawNpcs(npcEntityList,game.numberOfNpcEntitys);

                    DrawItemEntityList(ItemEntitylist,game.numberOfItemEntitys);
                    
                    DrawObjects(objectEntityList,game);

                    DrawPlayer(&player);

			    EndMode2D();

                DrawDialogue(game.activeDialogue);

                Drawpopup(game.activePopup);

                DrawChoiceMenu(*game.choiceMenu);
                
                DrawInventory(&player.inventory,(Vector2){100,100});

            }else if(game.currentScreen == END){
                DrawEndMenu(&game);
            }

		EndDrawing();
    }

    UnloadMusicStream(music);
    UnloadTexture(player.sprite.texture);
    FreeAllObjectEntitys(objectEntityList);
    free(map);
    free(npcEntityList);
    free(ItemEntitylist);
    free(objectEntityList);
    FreeNpcCatalog( );
    FreeItemCatalog();
    FreeTileCatalog();
    FreeObjectCatalog();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
