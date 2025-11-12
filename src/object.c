#include "object.h"

#include <raylib.h>
#include <player.h>
#include <math.h>
#include <tilemap.h>

/*
    Função para pegar a hitbox do sprite
    Simplesmente retorna um retangulo com a posição do sprite
    *Nota: a width e height estão sujeitas a mudança
    e são asssim devido o tamanho de cada frame da animação
*/
Rectangle GetObjectHitbox(Object self,float width,float height){
    return (Rectangle){
        .x = self.position.x -(width/2),
        .y = self.position.y-(height/2),
        .width = width,
        .height = height,
    };
}

    //Função para atualizar a posição X do sprite em relação a sua velocidade
void applyVelX(Object *self){
    self->position.x += self->velocity.x;
}

    //Função para atualizar a posição Y do sprite em relação a sua velocidade
void applyVelY(Object *self){
    self->position.y += self->velocity.y;
}

/*
    Função para checar a sobreposição em Y de um sprite e um retangulo 
    É feito o calculo da sobreposição dos dois lados de cada retangulo
    o lado em que a sobreposição for maior significa que não esta sobreposta    
    retornando o outro lado
*/
float CheckCollisionY(Rectangle hitbox,Rectangle collisionRectangle){

    if (CheckCollisionRecs(hitbox, collisionRectangle)) {
            
        float overlapUp  = (hitbox.y + hitbox.height) - collisionRectangle.y;
        float overlapDown = (collisionRectangle.y + collisionRectangle.height) - hitbox.y;

        if (overlapUp < overlapDown) {
            return -overlapUp;
        } else {
            return overlapDown;
        }
    }

    return 0;

}

/*
    Função para checar a sobreposição em X de um sprite e um retangulo
    A função calcula a sobreposição dos dois lados de cada retangulo
    o lado em que a sobreposição for maior significa que não esta sobreposta
    retornando a sobreposição do outro lado
*/
float CheckCollisionX(Rectangle hitbox,Rectangle collisionRectangle){

    if (CheckCollisionRecs(hitbox, collisionRectangle)) {
            
        float overlapLeft  = (hitbox.x + hitbox.width) - collisionRectangle.x;
        float overlapRight = (collisionRectangle.x + collisionRectangle.width) - hitbox.x;

        if (overlapLeft < overlapRight) {
            return -overlapLeft;
        } else {
            return overlapRight;
        }
    }

    return 0;

}

bool correctObjectCollisionX(Object *object,Object *collisionObject){
    float totalPush = 0;

    Rectangle collisionRectangle = GetObjectHitbox(*collisionObject,16,16);
    Rectangle objectHitbox = GetObjectHitbox(*object,20,25);

    float push = CheckCollisionX(objectHitbox,collisionRectangle);

    if (fabs(push) > fabs(totalPush)) totalPush = push;

    object->position.x += totalPush;

}

bool correctObjectCollisionY(Object *object,Object *collisionObject){
    float totalPush = 0;

    Rectangle collisionRectangle = GetObjectHitbox(*collisionObject,16,16);
    Rectangle objectHitbox = GetObjectHitbox(*object,20,25);

    float push = CheckCollisionY(objectHitbox,collisionRectangle);

    if (fabs(push) > fabs(totalPush)) totalPush = push;

    object->position.y += totalPush;

}
bool ObjectWillCollideWithTilesX(Object *object, unsigned char *map, float deltaX) {
    Rectangle hitbox = GetObjectHitbox(*object, 16, 16);
    hitbox.x += deltaX;

    TileBounds tiles = GetTileBounds(hitbox);

    for (int y = tiles.top; y <= tiles.bottom; y++) {
        for (int x = tiles.left; x <= tiles.right; x++) {
            if (GetTileById(map[y * MAP_COLS + x]).isSolid) return true;
        }
    }
    return false;
}

bool ObjectWillCollideWithTilesY(Object *object, unsigned char *map, float deltaY) {
    Rectangle hitbox = GetObjectHitbox(*object, 16, 16);
    hitbox.y += deltaY;  

    TileBounds tiles = GetTileBounds(hitbox);

    for (int y = tiles.top; y <= tiles.bottom; y++) {
        for (int x = tiles.left; x <= tiles.right; x++) {
            if (GetTileById(map[y * MAP_COLS + x]).isSolid) return true;
        }
    }
    return false;
}

void PushObjectX(Object *object, Player *player, unsigned char *map) {
    if (!object->isPushable) return;

    correctObjectCollisionX(object, &player->object);

    Rectangle objHitbox = GetObjectHitbox(*object, 16, 16);
    TileBounds tiles = GetTileBounds(objHitbox);
    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){
            Tile tile = GetTileById(map[y * MAP_COLS + x]);
            if (tile.isSolid) {
                Rectangle tileRect = CreateTileRectangle((Vector2){x, y});
                float push = CheckCollisionX(objHitbox, tileRect);
                if (fabs(push) > fabs(totalPush)) totalPush = push;
            }
        }
    }

    if(totalPush != 0){
        object->position.x += totalPush;         
        player->object.position.x += totalPush;  
        player->object.velocity.x = 0;           
    }
}

void PushObjectY(Object *object, Player *player, unsigned char *map) {
    if (!object->isPushable) return;

    correctObjectCollisionY(object, &player->object);

    Rectangle objHitbox = GetObjectHitbox(*object, 16, 16);
    TileBounds tiles = GetTileBounds(objHitbox);
    float totalPush = 0;

    for(int y = tiles.top; y <= tiles.bottom; y++){
        for(int x = tiles.left; x <= tiles.right; x++){
            Tile tile = GetTileById(map[y * MAP_COLS + x]);
            if (tile.isSolid) {
                Rectangle tileRect = CreateTileRectangle((Vector2){x, y});
                float push = CheckCollisionY(objHitbox, tileRect);
                if (fabs(push) > fabs(totalPush)) totalPush = push;
            }
        }
    }

    if(totalPush != 0){
        object->position.y += totalPush;         
        player->object.position.y += totalPush;  
        player->object.velocity.y = 0;
    }
}

void DrawObject(Object *object){

    Rectangle objectHitbox = GetObjectHitbox(*object,16,16);

    DrawRectangleRec(objectHitbox,GREEN);

}
