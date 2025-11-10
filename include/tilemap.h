#ifndef TILEMAP_H
#define TILEMAP_H

#include <raylib.h>
#include <sprite.h>
#include <object.h>

#define NUMBER_OF_TILES 4

#define HEIGHT 1024 //altura do mapa(recomendado que seja divisivel pelo tamanho do tile)
#define WIDTH 1536 // largura do mapa(recomendado que seja divisivel pelo tamanho do tile)
#define TILE_SIZE 16 //tamanho dos tiles(recomando que seja divisivel por 8)

#define MAP_ROWS (HEIGHT / TILE_SIZE) //Quantidade de linhas dos mapas
#define MAP_COLS (WIDTH / TILE_SIZE) //Quantidade de colunas dos mapas

typedef struct Tile
{
    unsigned char id;
    bool isSolid;
    Sprite sprite;  
    
}Tile;


typedef struct 
{
    int left;
    int right;
    int top;
    int bottom;

}TileBounds;

void WriteMap(const char *fileName);

//Função para ler o arquivo de um mapa
unsigned char *ReadMap(const char *fileName);

//Função para desenhar o mapa
void DrawMap(unsigned char *self);

//Função para gerar colisão no Y dos tiles colisiveis 
void CheckTilesCollisionY(Object *object, unsigned char *map);

//Função para gerar colisão no X dos tiles colisiveis 
void CheckTilesCollisionX(Object *object, unsigned char *map);

#endif 