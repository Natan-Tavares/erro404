#include "item.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <player.h>
#include <sprite.h>
#include <animation.h>
#include <object.h>
#include <game.h>
#include <popup.h>
#include <utils.h>

Item *GetItemCatalog(){
    static Item catalog[NUMBER_OF_ITEMS];

    static bool initialized = false;
    if(!initialized){
        catalog[0] = (Item){.id = 0,.nome="Moeda",.sprite= (Sprite)
            {
            .texture=LoadTexture("resources/textures/coin.png"),
            .animation = (animation){.first = 0 ,.last = 6,.durationLeft = 0.1,.numFramesPerAxle={6,1},.speed=0.1,.state = IDLE}
            }};
        catalog[1] = (Item){.id = 1,.nome="chave",.sprite= (Sprite){
                .texture = LoadTexture("resources/textures/Key.png"),
                .animation = (animation){.numFramesPerAxle={1,1},.state = IDLE}
            }};
        catalog[2] = (Item){.id=2,.nome="rato",.sprite= (Sprite){
            .texture = LoadTexture("resources/textures/rat.png"),
            .animation = (animation){.first=0,.last=2,.numFramesPerAxle={2,1},.speed=0.5,.state=0}
            }};
        catalog[3] = (Item){.id=3,.nome="Artefato Vermelho",.sprite= (Sprite){
            .texture = LoadTexture("resources/textures/artefato_vermelho.png"),
            .animation = (animation){.first=0,.last=1,.numFramesPerAxle={1,1},.speed=0.5,.state=0}
            }};
        catalog[4] = (Item){.id=4,.nome="Artefato Verde",.sprite= (Sprite){
            .texture = LoadTexture("resources/textures/artefato_verde.png"),
            .animation = (animation){.first=0,.last=1,.numFramesPerAxle={1,1},.state=0}
            }};
        catalog[5] = (Item){.id=5,.nome="Artefato Azul",.sprite= (Sprite){
            .texture = LoadTexture("resources/textures/artefato_azul.png"),
            .animation = (animation){.numFramesPerAxle={1,1},.state=0}
            }};

        initialized = true;
    }

    return catalog;
}

void FreeItemCatalog(){
    Item *itemCatalog = GetItemCatalog();
    for(int i = 0;i < NUMBER_OF_ITEMS;i++) UnloadTexture(itemCatalog[i].sprite.texture);
}

Item *GetItemById(int id){

    Item* itemCatalog = GetItemCatalog();

    for(int i = 0;i < NUMBER_OF_ITEMS;i++){
        if(itemCatalog[i].id == id){
            return &itemCatalog[i];
        }
    }

    printf("[GetItemById] Item ID %d não encontrado!\n", id);
    return NULL;

}

int CountItems(FILE *file){

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line,sizeof(line),file))
    {

        if(line[0] == '#') count++;
    }
    
    rewind(file);
    return count;

}

void HandleItemId(ItemEntity *itemEntity,const char *value){
    itemEntity->itemId = atoi(value);
    itemEntity->colected = false;
}

void HandleItemPosition(ItemEntity *itemEntity,const char *value){
    sscanf(value,"%f,%f",&(itemEntity->position.x),&(itemEntity->position.y));
}

void FillItemValues(ItemEntity *itemEntity, char *line) {

    static const ItemFieldHandler itemHandlers[] = {
        {"id:",HandleItemId},
        {"position:",HandleItemPosition},
    };

    static const int itemHandleCount = sizeof(itemHandlers) / sizeof(itemHandlers[0]);

    for(int i = 0;i < itemHandleCount;i++){
        if(!strncmp(line,itemHandlers[i].key,strlen(itemHandlers[i].key))){
            itemHandlers[i].handle(itemEntity,line + strlen(itemHandlers[i].key));
            return;
        }
    }

}

void ReadItems(ItemEntity *itemEntityList,FILE *file){

    char line[MAX_LINE_LENGTH];
    int index = -1;

    while(fgets(line,sizeof(line),file))
    {
        if(strlen(line) == 0) continue; 

        if(line[0] == '#'){ 
            index++;
            continue;  
        } 

        line[strcspn(line, "\n")] = '\0';

        FillItemValues(&itemEntityList[index],line); 
    }
}

ItemEntity *LoadItems(const char* filename,int *numberOfItems){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfItems = CountItems(file);

    ItemEntity *itemEntityList = malloc(sizeof(ItemEntity) * (*numberOfItems));

    ReadItems(itemEntityList,file); 

    fclose(file);
    return itemEntityList;
}

Rectangle GetItemHitbox(ItemEntity *itemEntity){
    return GetObjectHitbox((ObjectEntity){.position = itemEntity->position},20,20);
}

void CollectItem(ItemEntity *itemEntity,Player *player,GameManager *gameManager){
    itemEntity->colected = true;

    Inventory *inventory = &player->inventory;

    AddItemToInventory(&player->inventory,itemEntity->itemId,1);
    PreDoneCollectItemPopup(itemEntity->itemId,&gameManager->activePopup);
}



void TryToCollect(ItemEntity *itemEntity,Player *player,GameManager *gameManager){
    Rectangle itemHitbox = GetItemHitbox(itemEntity);
    Rectangle playerHitbox = GetObjectHitbox(player->object,20,25);

    if (CheckCollisionRecs(playerHitbox,itemHitbox) && !itemEntity->colected){
        
        CollectItem(itemEntity,player,gameManager);

    }

}

void DrawItem(ItemEntity *itemEntity,Vector2 position){
    Item *item = GetItemById(itemEntity->itemId);
    if(!item) return;

    Rectangle animationFrame = GetAnimationFrame(item->sprite,item->sprite.animation.numFramesPerAxle);
    Rectangle dest = GetAnimationFrame(item->sprite,item->sprite.animation.numFramesPerAxle);
    dest.x = position.x;
    dest.y = position.y;
    dest.width *= 0.5;
    dest.height *= 0.5;

    DrawTexturePro(
        item->sprite.texture,
        animationFrame,
        dest,
        (Vector2){0,0},
        0,
        WHITE
    );

}

void DrawItemEntityList(ItemEntity *ItemEntityList,int numberOfItemEntitys){
    for(int i = 0;i < numberOfItemEntitys;i++){        
        ItemEntity *itemEntity = &ItemEntityList[i];

        if(!itemEntity->colected) 
            DrawItem(itemEntity,itemEntity->position);
    }
}

void UpdateItemEntity(ItemEntity *ItemEntityList,GameManager *gameManager,Player *player){
    for(int i = 0;i < gameManager->numberOfItemEntitys;i++){
        ItemEntity *itemEntity = &ItemEntityList[i];

        if(!itemEntity->colected) TryToCollect(itemEntity,player,gameManager);

    }
}

void UpdateItemsAnimation(){
    for(int i = 0;i < NUMBER_OF_ITEMS;i++){
        Item *item = GetItemById(i);

        UpdateAnimation(&item->sprite.animation);

    }

}