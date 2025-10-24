#include <raylib.h>
#include <animation.h>
#include <player.h>
#include <sprite.h>

int main()
{
    InitWindow(600,400,"main");

    Texture textura = LoadTexture("resources/soldier.png");

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

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        MovePlayer(&sprite);

        applyVelX(&sprite);
        checkCollisionX(&sprite,colisao);

        applyVelY(&sprite);
        checkCollisionY(&sprite,colisao);

        PlayerStatemachine(&sprite);

        UpdateAnimation(&(sprite.animation));

        BeginDrawing();

            ClearBackground(SKYBLUE);

            DrawPlayer(&sprite,textura);

            DrawRectangleRec(colisao,GREEN);

        EndDrawing();
    }
    
    UnloadTexture(textura);
    
    return 0;
}
