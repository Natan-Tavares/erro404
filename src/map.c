#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define HEIGHT 512
#define WIDTH 768
#define TILE_SIZE 16

#define MAP_ROWS (HEIGHT / TILE_SIZE)
#define MAP_COLS (WIDTH / TILE_SIZE)

void WriteMap(void) {

    unsigned char map[MAP_ROWS][MAP_COLS] = {0};

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            map[y][x] = 1;
        }
    }

    FILE *file = fopen("resources/maps/map.bin", "wb");
    if (!file) {
        perror("Erro ao abrir arquivo para escrita");
        return;
    }

    fwrite(map, sizeof(unsigned char), MAP_ROWS * MAP_COLS, file);
    fclose(file);
}

unsigned char *ReadMap(const char *fileName) {

    unsigned char *map = malloc(sizeof(unsigned char) * MAP_ROWS * MAP_COLS);

    FILE *file = fopen(fileName, "rb");
    if (!file) {
        perror("Erro ao abrir o mapa");
        free(map);
        return NULL;
    }

    fread(map, sizeof(unsigned char), MAP_ROWS * MAP_COLS, file);
    fclose(file);

    return map;
}

Color CheckTile(unsigned char tile) {
    switch (tile) {
        case 0: return LIGHTGRAY;
        case 1: return DARKGRAY;
        case 2: return RED;
        case 3: return BLUE;
        default: return BLACK;
    }
}

void DrawMap(unsigned char *self) {

    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {

            unsigned char tile = self[y * MAP_COLS + x];
            Color color = CheckTile(tile);

            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
        }
    }
}


