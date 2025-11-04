#ifndef TILEMAP_H
#define TILEMAP_H

#include <raylib.h>
#include <sprite.h>

#define HEIGHT 512 //altura dos mapas(recomendado que seja divisivel pelo tamanho do tile)
#define WIDTH 768 // largura dos mapas(recomendado que seja divisivel pelo tamanho do tile)
#define TILE_SIZE 16 //tamanho dos tiles(recomando que seja divisivel por 8)

#define MAP_ROWS (HEIGHT / TILE_SIZE) //Quantidade de linhas dos mapas
#define MAP_COLS (WIDTH / TILE_SIZE) //Quantidade de colunas dos mapas

typedef struct 
{
    int left;
    int right;
    int top;
    int bottom;

}TileBounds;

//Função para ler o arquivo de um mapa
unsigned char *ReadMap(const char *fileName);

//Função para desenhar o mapa
void DrawMap(unsigned char *self);

//Função para gerar colisão no Y dos tiles colisiveis 
void CheckTilesCollisionY(Sprite *sprite, unsigned char *map);

//Função para gerar colisão no X dos tiles colisiveis 
void CheckTilesCollisionX(Sprite *sprite, unsigned char *map);

#endif 