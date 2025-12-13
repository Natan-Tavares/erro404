#include "tilemap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include <object.h>
#include <game.h>
#include <utils.h>

#include <sprite.h>

Tile *GetTileCatalog(){
    static Tile catalog[NUMBER_OF_TILES];

    static bool initialized = false;

    if(!initialized){
        catalog[0] = (Tile){0,false,(Sprite){0}};
        catalog[1] = (Tile){1,false,(Sprite){.texture=LoadTexture("resources/textures/grass.png")}};
        catalog[2] = (Tile){2,true,(Sprite){.texture=LoadTexture("resources/textures/brick.png")}};
        catalog[3] = (Tile){3,true,(Sprite){}};
        catalog[4] = (Tile){4,false,(Sprite){.texture=LoadTexture("resources/textures/floor.png")}};
        
        initialized = true;
    }
    return catalog;
}

Tile GetTileById(unsigned char id){
    Tile *catalog = GetTileCatalog();

    for(int i = 0; i < NUMBER_OF_TILES;i++){
        if(catalog[i].id == id) return catalog[i];
    }

    return catalog[0];

}

void FreeTileCatalog(){
    Tile *catalog = GetTileCatalog();

    for(int i = 0;i < NUMBER_OF_TILES;i++){
        if(catalog[i].sprite.texture.id != 0) UnloadTexture(catalog[i].sprite.texture);
    }

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

            Tile tile = GetTileById(self[y * MAP_COLS + x]);

            if(tile.sprite.texture.id == 0) continue;

            DrawTexturePro(
                tile.sprite.texture,
                (Rectangle){0,0,16,16},
                (Rectangle){x*TILE_SIZE,y*TILE_SIZE,TILE_SIZE,TILE_SIZE},
                (Vector2){0,0},
                0.0,
                WHITE
            );

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
void CheckTilesCollisionX(ObjectEntity *object, unsigned char *map) {

    Rectangle hitbox = GetObjectHitbox(*object,16,16);
    TileBounds tiles = GetTileBounds(hitbox);

    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){

            if(GetTileById(map[y * MAP_COLS + x]).isSolid == true){

                Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});

                float push = CheckCollisionX(hitbox,tileRectangle);

                if (fabs(push) > fabs(totalPush)) totalPush = push;
        
            }
        }
    }

    object->position.x += totalPush;
}


/*
    Função para pegar os tiles em que o sprite esta em cima,
    checar se eles são colisiveis, se forem criar um retangulo
    e gerar colisão entre esse retangulo e o Y do sprite
    *Nota: função suscetível a mudança quando os tiles forem definidos,
    para mudar a forma de checar os tiles colisiveis 
*/
void CheckTilesCollisionY(ObjectEntity *object, unsigned char *map) {

    Rectangle hitbox = GetObjectHitbox(*object,16,16);
    TileBounds tiles = GetTileBounds(hitbox);

    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){

            if(GetTileById(map[y * MAP_COLS + x]).isSolid == true){

                Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});

                float push = CheckCollisionY(hitbox,tileRectangle);

                if (fabs(push) > fabs(totalPush)) totalPush = push;

            }
        }
    }
    
    object->position.y += totalPush;

}
