#ifndef TILEMAP_H
#define TILEMAP_H

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sprite.h>

#define HEIGHT 512
#define WIDTH 768
#define TILE_SIZE 16

#define MAP_ROWS (HEIGHT / TILE_SIZE)
#define MAP_COLS (WIDTH / TILE_SIZE)

unsigned char *ReadMap(const char *fileName);

void DrawMap(unsigned char *self);

void CheckTilesCollisionY(Sprite *sprite, unsigned char *map);

void CheckTilesCollisionX(Sprite *sprite, unsigned char *map);

#endif 