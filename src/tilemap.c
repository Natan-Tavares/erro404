#include "tilemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <sprite.h>
#include <player.h>

/*
    Função para checar se foi possivel manipular um arquivo.
*/
bool CheckFile(FILE *file){
    if(file){
        return true;
    }
    perror("Erro No arquivo:");
    return false;
}

/*
    Função interna para casos especificos que escrev valores default
     em um arquivo binario de mapa.
*/
void WriteMap(const char *fileName) {

    unsigned char map[MAP_ROWS][MAP_COLS] = {0};

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map[y][x] = 1;
        }
    }

    FILE *file = fopen(fileName, "wb");

    if(CheckFile(file)){
        fwrite(map, sizeof(unsigned char), MAP_ROWS * MAP_COLS, file);
    }

    fclose(file);
}

/*
    Função para ler um arquivo em binario que contem os valores de cada tile 
    de um mapa, passando os valores para uma lista.
*/
unsigned char *ReadMap(const char *fileName) {

    unsigned char *map = malloc(sizeof(unsigned char) * MAP_ROWS * MAP_COLS);

    FILE *file = fopen(fileName, "rb");

    if (CheckFile(file))
    {
        fread(map, sizeof(unsigned char), MAP_ROWS * MAP_COLS, file);
        fclose(file);

        return map;
    }

    fclose(file);
    free(map);

    return NULL;
}

/*
    Função para checar qual o tile de acordo com o seu index.
*/
Color CheckTile(unsigned char tile) {
    switch (tile) {
        case 0: return LIGHTGRAY;
        case 1: return DARKGRAY;
        case 2: return RED;
        case 3: return BLUE;
        default: return BLACK;
    }
}

/*
    Função para desenhar o mapa de acordo com a posição de cada tile na lista
    e do tamanho do tile.
*/
void DrawMap(unsigned char *self) {

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {

            unsigned char tile = self[y * MAP_COLS + x];
            Color color = CheckTile(tile);

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
}

/*
    Função para pegar os tiles em que o sprite esta em cima,
    checar se eles são colisiveis, se forem criar um retangulo
    e gerar colisão entre esse retangulo e o Y do sprite.
    *Nota: função suscetível a mudança quando os tiles forem definidos,
    para mudar a forma de checar os tiles colisiveis.
*/
void CheckTilesCollisionX(Sprite *sprite, unsigned char *map) {

    Rectangle hitbox = GetSpriteHitbox(*sprite,20,25);

    int leftTile = hitbox.x / TILE_SIZE;
    int rightTile = (hitbox.x + hitbox.width - 1)/TILE_SIZE;
    int topTile = hitbox.y / TILE_SIZE;
    int bottomTile = (hitbox.y + hitbox.height - 1) / TILE_SIZE;

    for(int y = topTile; y <= bottomTile; y++){
        for(int x = leftTile; x <= rightTile; x++){

            if(map[y * MAP_COLS + x] == 3){

                Rectangle tileRectangle = (Rectangle){
                    .x = x * TILE_SIZE,
                    .y = y * TILE_SIZE,
                    .width = TILE_SIZE,
                    .height = TILE_SIZE
                };

                checkCollisionX(sprite,hitbox,tileRectangle);
            }
        }
    }
}


/*
    Função para pegar os tiles em que o sprite esta em cima,
    checar se eles são colisiveis, se forem criar um retangulo
    e gerar colisão entre esse retangulo e o Y do sprite
    *Nota: função suscetível a mudança quando os tiles forem definidos,
    para mudar a forma de checar os tiles colisiveis 
*/
void CheckTilesCollisionY(Sprite *sprite, unsigned char *map) {

    Rectangle hitbox = GetSpriteHitbox(*sprite,20,25);

    int leftTile = hitbox.x / TILE_SIZE;
    int rightTile = (hitbox.x + hitbox.width - 1)/TILE_SIZE;
    int topTile = hitbox.y / TILE_SIZE;
    int bottomTile = (hitbox.y + hitbox.height - 1) / TILE_SIZE;

    for(int y = topTile; y <= bottomTile; y++){
        for(int x = leftTile; x <= rightTile; x++){

            if(map[y * MAP_COLS + x] == 3){

                Rectangle tileRectangle = (Rectangle){
                    .x = x * TILE_SIZE,
                    .y = y * TILE_SIZE,
                    .width = TILE_SIZE,
                    .height = TILE_SIZE
                };

                checkCollisionY(sprite,hitbox,tileRectangle);
            }
        }
    }
}
