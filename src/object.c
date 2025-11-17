#include "object.h"

#include <string.h>
#include <raylib.h>
#include <player.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <tilemap.h>
#include <game.h>

Object *GetObjectCatalog(){
    static Object catalog[NUMBER_OF_OBJECTS];

    static bool isInitialized = false;
    if(!isInitialized){
        catalog[0] = (Object){.id = 0,.sprite= (Sprite){
                .texture = LoadTexture("resources/textures/box.png"),
                .animation = (animation){.numFramesPerAxle={1,1},.state = IDLE}
            }};
        isInitialized = true;
    }

    return catalog;
}

void FreeObjectCatalog(){
    Object *catalog = GetObjectCatalog();

    for(int i = 0;i < NUMBER_OF_OBJECTS;i++){
        UnloadTexture(catalog[i].sprite.texture);
    }
}

Object *GetObjectById(int id){
    Object *catalog = GetObjectCatalog();

    for(int i =0; i < NUMBER_OF_OBJECTS;i++){
        if(catalog[i].id == id){
            return &catalog[i];
        }
    }

    printf("[GetObjectById] Objeto ID %d não encontrado!\n", id);
    return NULL;
}
/*
    Função para pegar a hitbox do sprite
    Simplesmente retorna um retangulo com a posição do sprite
    *Nota: a width e height estão sujeitas a mudança
    e são asssim devido o tamanho de cada frame da animação
*/
Rectangle GetObjectHitbox(ObjectEntity self,float width,float height){
    return (Rectangle){
        .x = self.position.x -(width * 0.5),
        .y = self.position.y-(height * 0.5),
        .width = width,
        .height = height,
    };
}

    //Função para atualizar a posição X do sprite em relação a sua velocidade
void applyVelX(ObjectEntity *self){
    self->position.x += self->velocity.x;
}

    //Função para atualizar a posição Y do sprite em relação a sua velocidade
void applyVelY(ObjectEntity *self){
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

int CountObjectEntitys(FILE *file){

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line,sizeof(line),file))
    {
        if(line[0] == '#') count++;
    }
    
    rewind(file);
    return count;
}

void FillObjectValue(ObjectEntity *object,char *line){
    
    if(!strncmp(line,"id:",3)){
        object->ObjectId = atoi(line+3);
    }else if(!strncmp(line,"position:", 9)){
        sscanf(line+9,"%f,%f",&(object->position.x),&(object->position.y));
    }else if(!strncmp(line,"pushable:",9)){
        if(!(strncmp(line+9,"sim",3))){
            object->isPushable=true;
        }else{
            object->isPushable=false;
        }
    }

}

void ReadObjects(ObjectEntity *ObjectEntitys,FILE *file){

    char line[MAX_LINE_LENGTH];
    int index = -1;

    while (fgets(line,MAX_LINE_LENGTH,file))
    {
        if(strlen(line) == 0) continue;

        if(line[0] == '#'){
            index++;
            continue;
        }
        line[strcspn(line,"\n")] = '\0';

        FillObjectValue(&ObjectEntitys[index],line);
    }
    
}

ObjectEntity *LoadObjects(const char *filename,int *numberOfObjects){
    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfObjects = CountObjectEntitys(file);

    ObjectEntity *objects = malloc(sizeof(ObjectEntity) * (*numberOfObjects));

    ReadObjects(objects,file);

    fclose(file);
    return objects;
}

void DrawObjects(ObjectEntity *objectList,GameManager gameManager){
    for(int i =0;i < gameManager.numberOfObjectEntitys;i++){
        Object *object = GetObjectById(objectList[i].ObjectId);


        Rectangle rectangleFrame = GetAnimationFrame(object->sprite,object->sprite.animation.numFramesPerAxle);
        Rectangle objectPosition = (Rectangle){
            .x=objectList[i].position.x,
            .y=objectList[i].position.y,
            .height=16, .width=16
        };
        
        DrawTexturePro(
            object->sprite.texture,
            rectangleFrame,
            objectPosition,
            (Vector2){8,8},
            0,
            WHITE
        );
    }
}

bool TryMoveObjectChain(ObjectEntity *objects,unsigned char *map,int count,int index,float pushX,float pushY) {
    ObjectEntity *b = &objects[index];

    float prevX = b->position.x;
    float prevY = b->position.y;

    b->position.x += pushX;
    b->position.y += pushY;

    Rectangle hit = GetObjectHitbox(*b, 16, 16);

    TileBounds tiles = GetTileBounds(hit);
    for (int ty = tiles.top; ty <= tiles.bottom; ty++) {
        for (int tx = tiles.left; tx <= tiles.right; tx++) {

            if (GetTileById(map[ty * MAP_COLS + tx]).isSolid == true) {
                b->position.x = prevX;
                b->position.y = prevY;
                return false;
            }
        }
    }

    for (int j = 0; j < count; j++) {
        if (j == index) continue;

        Rectangle other = GetObjectHitbox(objects[j], 16, 16);

        if (CheckCollisionRecs(hit, other)) {

            if (objects[j].isPushable) {

                if (!TryMoveObjectChain(objects, map, count, j, pushX, pushY)) {
                    b->position.x = prevX;
                    b->position.y = prevY;
                    return false;
                }

            } else {
                b->position.x = prevX;
                b->position.y = prevY;
                return false;
            }
        }
    }

    return true;
}

void ResolvePlayerVsObjectsX(Player *player, ObjectEntity *objects, unsigned char *map, int count) {
    ObjectEntity *p = &player->object;

    for (int i = 0; i < count; i++) {
        ObjectEntity *b = &objects[i];

        Rectangle ph = GetObjectHitbox(*p, 16, 16);
        Rectangle oh = GetObjectHitbox(*b, 16, 16);

        if (!CheckCollisionRecs(ph, oh)) continue;

        float push = CheckCollisionX(ph, oh);

        if (!b->isPushable) {
            p->position.x += push;
            continue;
        }

        if (!TryMoveObjectChain(objects, map, count, i, -push, 0)) {
            p->position.x += push;
        }
    }
}

void ResolvePlayerVsObjectsY(Player *player, ObjectEntity *objects, unsigned char *map, int count) {
    ObjectEntity *p = &player->object;

    for (int i = 0; i < count; i++) {
        ObjectEntity *b = &objects[i];

        Rectangle ph = GetObjectHitbox(*p, 16, 16);
        Rectangle oh = GetObjectHitbox(*b, 16, 16);

        if (!CheckCollisionRecs(ph, oh)) continue;

        float push = CheckCollisionY(ph, oh);

        // OBJETO SÓLIDO
        if (!b->isPushable) {
            p->position.y += push;
            continue;
        }

        if (!TryMoveObjectChain(objects, map, count, i, 0, -push)) {
            p->position.y += push;
        }
    }
}
