#include "tilemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <sprite.h>
#include <player.h>

/*
    Função para checar se foi possivel manipular um arquivo;
*/
bool CheckFile(FILE *file){
    if(file){
        return true;
    }
    perror("Erro No arquivo:");
    return false;
}

/*
    Função para escrever valores default em um arquivo binario de mapa;
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
    de um mapa, passando os valores para uma lista
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
    Função para checar o tile de acordo com o seu index
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

void CheckTilesCollisionX(Sprite *sprite, unsigned char *map) {

    Vector2 tile = GetPlayerTile(sprite);

    for(int i = -1;i < 3;i+=2){
        if(map[(int)(tile.y * MAP_COLS + (tile.x+i))] == 3){
            Rectangle tileRectangle = (Rectangle){
                .x = (tile.x+i) * TILE_SIZE,
                .y = tile.y * TILE_SIZE,
                .width = TILE_SIZE,
                .height = TILE_SIZE
            };
            checkCollisionX(sprite,tileRectangle);

        }
    }
}

void CheckTilesCollisionY(Sprite *sprite, unsigned char *map) {

    Vector2 tile = GetPlayerTile(sprite);

    for(int i = -1;i < 3;i+=2){
        if(map[(int)((tile.y+i) * MAP_COLS + tile.x)] == 3){
            Rectangle tileRectangle = (Rectangle){
                .x = tile.x * TILE_SIZE,
                .y = (tile.y+i) * TILE_SIZE,
                .width = TILE_SIZE,
                .height = TILE_SIZE
            };
            checkCollisionY(sprite,tileRectangle);
        }
    }
}
