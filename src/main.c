#include <raylib.h>
#include <animation.h>
#include <player.h>
#include <sprite.h>
#include <camera.h>
#include <tilemap.h>

#define WINDOW_WIDTH 1020
#define WINDOW_HEIGHT 810

int main()
{
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"main");

    Texture textura = LoadTexture("resources/textures/Soldier.png");

    Rectangle colisao = (Rectangle){
        .x = 100,
        .y = 100,
        .width = 30,
        .height = 30
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

    unsigned char *map = ReadMap("resources/maps/map.bin");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

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

        BeginDrawing();

            ClearBackground(SKYBLUE);

            BeginMode2D(camera);

                DrawMap(map);

                DrawPlayer(&sprite,textura);
        
            EndMode2D();
        EndDrawing();
    }
    
    UnloadTexture(textura);
    free(map);
    
    return 0;
}
