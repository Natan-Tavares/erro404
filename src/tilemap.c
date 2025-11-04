#include "tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#include <sprite.h>

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
    Função interna para casos especificos que escreve valores default
    em um arquivo binario de mapa.
    *Nota: Função especifica para testes de desenvolvimento(Não use guilherme)
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
    Função para criar um retangulo de acordo com a posição do tile
*/
Rectangle CreateTileRectangle(Vector2 position){

    return (Rectangle){
        .x = position.x *TILE_SIZE,
        .y = position.y *TILE_SIZE,
        .width = TILE_SIZE,
        .height = TILE_SIZE
    };
}

TileBounds GetTileBounds(Rectangle hitbox){

    return (TileBounds){

        .left = hitbox.x / TILE_SIZE,
        .bottom = (hitbox.y + hitbox.height - 1) / TILE_SIZE,
        .right = (hitbox.x + hitbox.width - 1)/TILE_SIZE,
        .top = hitbox.y / TILE_SIZE
    };

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
    TileBounds tiles = GetTileBounds(hitbox);

    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){

            if(map[y * MAP_COLS + x] == 3){

                Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});

                float push = CheckCollisionX(hitbox,tileRectangle);

                if (fabs(push) > fabs(totalPush)) totalPush = push;
        
            }
        }
    }

    sprite->position.x += totalPush;
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
    TileBounds tiles = GetTileBounds(hitbox);

    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){

            if(map[y * MAP_COLS + x] == 3){

                Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});

                float push = CheckCollisionY(hitbox,tileRectangle);

                if (fabs(push) > fabs(totalPush)) totalPush = push;

            }
        }
    }
    
    sprite->position.y += totalPush;

}
