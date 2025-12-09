#include "object.h"

#include <string.h>
#include <raylib.h>
#include <player.h>
#include <math.h>
#include <door.h>
#include <chest.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <tilemap.h>
#include <game.h>

Object *GetObjectCatalog(){
    static Object catalog[NUMBER_OF_OBJECTS];

    static bool isInitialized = false;
    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Object){.id = 0,.sprite = (Sprite){
                .texture = LoadTexture("resources/textures/box.png"),
                .animation = (animation){.numFramesPerAxle={1,1},.state = IDLE}
            }};
        catalog[1] = (Object){.id = 1,.Handlers=GetDoorHandlers,.OnInit=DoorInit,.OnUpdate=UpdateDoor,.sprite = (Sprite){
            .texture = LoadTexture("resources/textures/door.png"),
            .animation = (animation){.numFramesPerAxle={2,1},.state = IDLE}
            }};
        catalog[2] = (Object){.id = 2,.Handlers=GetDoorHandlers,.OnInit=DoorInit,.sprite = (Sprite){
            .texture = catalog[1].sprite.texture,
            .animation = (animation){.current=1,.numFramesPerAxle={2,1},.state = IDLE}
            }};
        catalog[3] = (Object){.id = 3,.Handlers=GetChestHandlers,.OnInit=ChestInit,.OnUpdate=UpdateChest,.sprite = (Sprite){
            .texture = LoadTexture("resources/textures/chest.png"),
            .animation = (animation){.numFramesPerAxle={2,1},.state = IDLE}
            }};
        catalog[4] = (Object){.id = 4,.sprite = (Sprite){
            .texture = catalog[3].sprite.texture,
            .animation = (animation){.current=1,.numFramesPerAxle={2,1},.state = IDLE}
            }};
        isInitialized = true;
    }

    return catalog;
}

void FreeObjectCatalog(){
    Object *catalog = GetObjectCatalog();

    for(int i = 0;i < NUMBER_OF_OBJECTS;i++){
        if(catalog[i].sprite.texture.id != 0)
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

void HandleObjectId(ObjectEntity *objectEntity,const char *value){
    objectEntity->ObjectId = atoi(value);
    objectEntity->isSolid = true;
    Object *object = GetObjectById(objectEntity->ObjectId);
    if(object->OnInit) object->OnInit(objectEntity);
}

void HandleObjectPosition(ObjectEntity *objectEntity,const char *value){
    sscanf(value,"%f,%f",&(objectEntity->position.x),&(objectEntity->position.y));
}

void HandleObjectIsSolid(ObjectEntity *objectEntity,const char *value){
    if(!strncmp(value,"sim",3)){
        objectEntity->isSolid = true;
        return;
    }

    objectEntity->isSolid = false;

}

void HandleObjectIsPushable(ObjectEntity *objectEntity,const char *value){
    if(!strncmp(value,"sim",3)){
        objectEntity->isPushable = true;
        return;
    }

    objectEntity->isPushable = false;

}

void FillObjectValue(ObjectEntity *objectEntity,char *line){

    static ObjectFieldHandler defaultObjectHandlers[] = {
        {"id:",HandleObjectId},
        {"position:",HandleObjectPosition},
        {"solid:",HandleObjectIsSolid},
        {"pushable:",HandleObjectIsPushable},
        {NULL,NULL},
    };

    for(int i = 0;defaultObjectHandlers[i].key != NULL;i++){
        if(!strncmp(defaultObjectHandlers[i].key,line,strlen(defaultObjectHandlers[i].key))){
            defaultObjectHandlers[i].handle(objectEntity,line+strlen(defaultObjectHandlers[i].key));        
            return;
        }
    }

    Object *object = GetObjectById(objectEntity->ObjectId);
    if(object->Handlers){
        ObjectFieldHandler *objectHandlers = object->Handlers();

        for (int i = 0; objectHandlers[i].key != NULL;i++){
            if(!strncmp(objectHandlers[i].key,line,strlen(objectHandlers[i].key))){
                objectHandlers[i].handle(objectEntity, line + strlen(objectHandlers[i].key));
                return;
            }
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

bool TryMoveObjectChain(ObjectEntity *objects, unsigned char *map, int numberOfObjectEntitys, ObjectEntity *currentObject, float pushX, float pushY) {

    if (!currentObject->isPushable) return false;

    Vector2 originalPositions[numberOfObjectEntitys];
    for (int i = 0; i < numberOfObjectEntitys; i++) originalPositions[i] = objects[i].position;

    currentObject->position.x += pushX;
    currentObject->position.y += pushY;

    Rectangle currentObjectHitbox = GetObjectHitbox(*currentObject, 16, 16);
    TileBounds tiles = GetTileBounds(currentObjectHitbox);
    for (int ty = tiles.top; ty <= tiles.bottom; ty++) {
        for (int tx = tiles.left; tx <= tiles.right; tx++) {
            if (GetTileById(map[ty * MAP_COLS + tx]).isSolid) {
                for (int i = 0; i < numberOfObjectEntitys; i++) objects[i].position = originalPositions[i];
                return false;
            }
        }
    }

    for (int j = 0; j < numberOfObjectEntitys; j++) {
        ObjectEntity *otherObject = &objects[j];

        if (otherObject == currentObject) continue;

        Rectangle otherObjectHitbox = GetObjectHitbox(objects[j], 16, 16);

        if (CheckCollisionRecs(currentObjectHitbox, otherObjectHitbox)) {
            if (!otherObject->isPushable) {
                for (int i = 0; i < numberOfObjectEntitys; i++) objects[i].position = originalPositions[i];
                return false;
            }

            if (!TryMoveObjectChain(objects, map, numberOfObjectEntitys, otherObject, pushX, pushY)) {
                for (int i = 0; i < numberOfObjectEntitys; i++) objects[i].position = originalPositions[i];
                return false;
            }
        }
    }

    return true;
}

void ResolvePlayerVsObjectsX(Player *player, ObjectEntity *objects, unsigned char *map, int numberOfObjectEntitys) {
    ObjectEntity *playerObject = &player->object;

    for (int i = 0; i < numberOfObjectEntitys; i++) {
        ObjectEntity *object = &objects[i];

        Rectangle playerHitbox = GetObjectHitbox(*playerObject, 16, 16);
        Rectangle objectHitbox = GetObjectHitbox(*object, 16, 16);

        if (!CheckCollisionRecs(playerHitbox, objectHitbox)) continue;

        float push = CheckCollisionX(playerHitbox,objectHitbox);

        if (!object->isPushable) {
            if(object->isSolid) playerObject->position.x += push;
            continue;
        }

        if (!TryMoveObjectChain(objects, map, numberOfObjectEntitys, object, -push, 0)) {
            playerObject->position.x += push;
        }
    }
}

void ResolvePlayerVsObjectsY(Player *player, ObjectEntity *objects, unsigned char *map, int count) {
    ObjectEntity *playerObject = &player->object;

    for (int i = 0; i < count; i++) {
        ObjectEntity *object = &objects[i];

        Rectangle playerHitbox = GetObjectHitbox(*playerObject, 16, 16);
        Rectangle objectHitbox = GetObjectHitbox(*object, 16, 16);

        if (!CheckCollisionRecs(playerHitbox, objectHitbox)) continue;

        float push = CheckCollisionY(playerHitbox, objectHitbox);

        if (!object->isPushable) {
            if(object->isSolid) playerObject->position.y += push;
            continue;
        }

        if (!TryMoveObjectChain(objects, map, count, object, 0, -push)) {
            playerObject->position.y += push;
        }
    }
}

void DrawObjectInteract(ObjectEntity *objectEntityList,GameManager gameManager){
    if(!gameManager.activeObject) return;

    DrawRectangle(100,600,400,120,Fade(BLACK,0.7));
        DrawText("Tentar abrir?",120,620,20,WHITE);        

    const char *options[] = {"Sim","Não"};
    for(int i = 0;i < 2;i++){
        Color color = (i == gameManager.selectedOption ) ? YELLOW : WHITE;
        DrawText(options[i],140,650 + i * 25,20,color);
    }

}

void UpdateObjectEntitys(ObjectEntity *objectEntitys,Player *player,GameManager *gameManager){
    for(int i = 0;i < gameManager->numberOfObjectEntitys ;i++){
        Object *object = GetObjectById(objectEntitys[i].ObjectId);

        if(object->OnUpdate){
            object->OnUpdate(&(objectEntitys[i]),player,gameManager);
        }

    }
}

void FreeObjectEntity(ObjectEntity *object){
    free(object->data);
}

void FreeAllObjectEntitys(ObjectEntity *objectEntitys){
    int objectAmount = sizeof(objectEntitys) / sizeof(objectEntitys[0]); 
    for(int i = 0;i < objectAmount;i++){
        FreeObjectEntity(&(objectEntitys[i]));
    }
}