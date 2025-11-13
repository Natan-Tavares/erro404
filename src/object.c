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

static inline bool IsValidTile(int x, int y) {
    return (x >= 0 && x < MAP_COLS && y >= 0 && y < MAP_ROWS);
}

static inline void ApplyPushX(ObjectEntity *a, ObjectEntity *b, float push) {
    if (a->isPushable && b->isPushable) {
        a->position.x += push * 0.5f;
        b->position.x -= push * 0.5f;
    } else if (a->isPushable) {
        a->position.x += push;
    } else if (b->isPushable) {
        b->position.x -= push;
    }
    a->velocity.x = 0;
    b->velocity.x = 0;
}

static inline void ApplyPushY(ObjectEntity *a, ObjectEntity *b, float push) {
    if (a->isPushable && b->isPushable) {
        a->position.y += push * 0.5f;
        b->position.y -= push * 0.5f;
    } else if (a->isPushable) {
        a->position.y += push;
    } else if (b->isPushable) {
        b->position.y -= push;
    }
    a->velocity.y = 0;
    b->velocity.y = 0;
}

static void ResolveObjectVsObject(ObjectEntity *a, ObjectEntity *b) {
    Rectangle hitA = GetObjectHitbox(*a, 16, 16);
    Rectangle hitB = GetObjectHitbox(*b, 16, 16);

    if (!CheckCollisionRecs(hitA, hitB)) return;

    float pushX = CheckCollisionX(hitA, hitB);
    float pushY = CheckCollisionY(hitA, hitB);

    if (fabsf(pushX) < fabsf(pushY)) {
        ApplyPushX(a, b, pushX);
    } else {
        ApplyPushY(a, b, pushY);
    }
}

static void ResolveAllObjects(ObjectEntity *objects, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            ResolveObjectVsObject(&objects[i], &objects[j]);
        }
    }
}

static void ResolveObjectVsMap(ObjectEntity *obj, unsigned char *map) {
    if (!obj || !map) return;

    Rectangle hit = GetObjectHitbox(*obj, 16, 16);
    TileBounds bounds = GetTileBounds(hit);

    float totalPushX = 0;
    float totalPushY = 0;

    for (int y = bounds.top; y <= bounds.bottom; y++) {
        for (int x = bounds.left; x <= bounds.right; x++) {
            if (!IsValidTile(x, y)) continue;

            Tile tile = GetTileById(map[y * MAP_COLS + x]);
            if (!tile.isSolid) continue;

            Rectangle tileRect = CreateTileRectangle((Vector2){x, y});

            if (CheckCollisionRecs(hit, tileRect)) {
                float pushX = CheckCollisionX(hit, tileRect);
                float pushY = CheckCollisionY(hit, tileRect);

                if (fabsf(pushX) < fabsf(pushY)) {
                    totalPushX = pushX;
                    obj->position.x += totalPushX;
                    obj->velocity.x = 0;
                    hit = GetObjectHitbox(*obj, 16, 16);
                } else {
                    totalPushY = pushY;
                    obj->position.y += totalPushY;
                    obj->velocity.y = 0;
                    hit = GetObjectHitbox(*obj, 16, 16);
                }
            }
        }
    }
}

void ResolvePlayerVsObjects(Player *player, ObjectEntity *objects,unsigned char *map, int count) {
    ObjectEntity *a = &player->object;

    for (int i = 0; i < count; i++) {
        ObjectEntity *b = &objects[i];

        Rectangle hitA = GetObjectHitbox(*a, 16, 16);
        Rectangle hitB = GetObjectHitbox(*b, 16, 16);

        if (!CheckCollisionRecs(hitA, hitB)) continue;

        float pushX = CheckCollisionX(hitA, hitB);
        float pushY = CheckCollisionY(hitA, hitB);

        bool resolveX = fabsf(pushX) < fabsf(pushY);

        // --- EIXO X ---
        if (resolveX) {
            if (!b->isPushable) {
                // objeto fixo → player é bloqueado completamente
                a->position.x += pushX;
                a->velocity.x = 0;
            } else {
                // tenta mover o objeto empurrável
                float prevX = b->position.x;
                b->position.x -= pushX;

                // rechecagem para evitar empurrar dentro de outro obstáculo
                Rectangle newHitB = GetObjectHitbox(*b, 16, 16);
                bool stillColliding = false;

                for (int j = 0; j < count; j++) {
                    if (j == i) continue;
                    Rectangle other = GetObjectHitbox(objects[j], 16, 16);
                    if (CheckCollisionRecs(newHitB, other)) {
                        stillColliding = true;
                        break;
                    }
                }

                TileBounds tiles = GetTileBounds(newHitB);
            
                for(int y = tiles.top; y <= tiles.bottom; y++){
                    for(int x = tiles.left; x <= tiles.right; x++){

                        if(GetTileById(map[y * MAP_COLS + x]).isSolid == true){

                           Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});
                            if (CheckCollisionRecs(newHitB, tileRectangle)) {
                                stillColliding = true;
                                break;
                            }
                        }
                    }
                }

                if (stillColliding) {
                    // caixa não pode se mover → player é bloqueado
                    b->position.x = prevX;
                    a->position.x += pushX;
                    a->velocity.x = 0;
                } else {
                    // caixa se move normalmente
                    b->velocity.x = 0;
                }
            }
        }

        // --- EIXO Y ---
        else {
            if (!b->isPushable) {
                a->position.y += pushY;
                a->velocity.y = 0;
            } else {
                float prevY = b->position.y;
                b->position.y -= pushY;

                Rectangle newHitB = GetObjectHitbox(*b, 16, 16);
                bool stillColliding = false;

                for (int j = 0; j < count; j++) {
                    if (j == i) continue;
                    Rectangle other = GetObjectHitbox(objects[j], 16, 16);
                    if (CheckCollisionRecs(newHitB, other)) {
                        stillColliding = true;
                        break;
                    }
                }
                TileBounds tiles = GetTileBounds(newHitB);
            
                for(int y = tiles.top; y <= tiles.bottom; y++){
                    for(int x = tiles.left; x <= tiles.right; x++){

                        if(GetTileById(map[y * MAP_COLS + x]).isSolid == true){

                           Rectangle tileRectangle = CreateTileRectangle((Vector2){x,y});
                            if (CheckCollisionRecs(newHitB, tileRectangle)) {
                                stillColliding = true;
                                break;
                            }
                        }
                    }
                }

                if (stillColliding) {
                    b->position.y = prevY;
                    a->position.y += pushY;
                    a->velocity.y = 0;
                } else {
                    a->velocity.y = 0;
                    b->velocity.y = 0;
                }
            }
        }
    }
}

void UpdateObjectsPhysics(ObjectEntity *objects, int count, unsigned char *map) {
    if (!objects || count <= 0 || !map) return;

    // Atualiza movimento e colisão de cada objeto
    for (int i = 0; i < count; i++) {
        // 1️⃣ aplica velocidade
        applyVelX(&objects[i]);
        applyVelY(&objects[i]);

        // 2️⃣ resolve colisão com o mapa
        ResolveObjectVsMap(&objects[i], map);
    }

    // 3️⃣ resolve colisões entre objetos
    ResolveAllObjects(objects, count);
}
