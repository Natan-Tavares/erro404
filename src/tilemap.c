#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define WIDTH 38
#define HEIGHT 25
#define TILE_SIZE 16

int *LoadMap(const char *fileName){

    int *map = malloc( HEIGHT * WIDTH * sizeof(int));

    FILE *file = fopen(fileName,"r");

    if(file == NULL){
        printf("Não foi possivel abrir o arquivo: %s", fileName); 
        return NULL;
    } 

    for(int y = 0;y < HEIGHT;y++){
        for(int x = 0; x < WIDTH;x++){

            char c = fgetc(file);
            if (c == '\n') c = fgetc(file);

        map[y*WIDTH + x] = c - '0';
        }
    }

    fclose(file);
    return map;

}

int *DrawMap(int *self){
        for(int y = 0;y < HEIGHT;y++){
            for(int x = 0; x < WIDTH;x++){
         
                int tile = self[y * WIDTH + x];
                Color color;

                if (tile == 1)
                    color = DARKGRAY; 
                else if (tile == 0)
                    color = LIGHTGRAY;
                else if (tile == 2)
                    color = BLUE;
                else if (tile == 3)
                    color = RED;
                else
                    color = BLACK; 

                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);         
            }
        }
    free(self);
}


int main()
{

    InitWindow(600,400,"tilemap");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        int *map = LoadMap("tilemap.txt");

        BeginDrawing();

        DrawMap(map);

        ClearBackground(SKYBLUE);

        EndDrawing();

    }
    return 0;
}
