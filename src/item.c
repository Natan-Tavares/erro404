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
#include <utils.h>

Item *GetItemCatalog(){
    static Item itemCatalog[NUMBER_OF_ITEMS];

    static bool initialized = false;
    if(!initialized){
        itemCatalog[0] = (Item){.id = 0,.nome="Moeda",.sprite= (Sprite)
            {
                .texture=LoadTexture("resources/textures/coin.png"),
                .animation = (animation){.first = 0 ,.last = 6,.durationLeft = 0.1,.numFramesPerAxle={6,1},.speed=0.1,.state = IDLE}
            }};
        itemCatalog[1] = (Item){.id = 1,.nome="chave",.sprite= (Sprite){
                .texture = LoadTexture("resources/textures/Key.png"),
                .animation = (animation){.numFramesPerAxle={1,1},.state = IDLE}
            }};

        initialized = true;
    }

    return itemCatalog;
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

void FillItemValues(ItemEntity *itemEntity, char *line) {

    if (!strncmp(line, "id:", 3)) {
        itemEntity->itemId = atoi(line + 3);
        itemEntity->colected = false;

    }else if(!strncmp(line,"position:",9)){
        sscanf(line+9,"%f,%f",&(itemEntity->position.x),&(itemEntity->position.y));
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

void CheckCollect(ItemEntity *itemEntity,Player *player){
    Rectangle itemHitbox = GetItemHitbox(itemEntity);
    Rectangle playerHitbox = GetObjectHitbox(player->object,20,25);

    if (CheckCollisionRecs(playerHitbox,itemHitbox) && !itemEntity->colected){
        itemEntity->colected = true;

        Inventory *inventory = &player->inventory;

        AddItemToInventory(&player->inventory,itemEntity->itemId,1);
        
    }

}

void DrawItem(ItemEntity *itemEntity,Vector2 position){
    Item *item = GetItemById(itemEntity->itemId);
    if(!item) return;

    Rectangle animationFrame = GetAnimationFrame(item->sprite,item->sprite.animation.numFramesPerAxle);
    Rectangle dest = GetAnimationFrame(item->sprite,item->sprite.animation.numFramesPerAxle);
    dest.x = position.x;
    dest.y = position.y;

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

void UpdateItemEntity(ItemEntity *ItemEntityList,int numberOfItemEntitys,Player *player){
    for(int i = 0;i < numberOfItemEntitys;i++){
        ItemEntity *itemEntity = &ItemEntityList[i];

        if(!itemEntity->colected) CheckCollect(itemEntity,player);

    }
}

void UpdateItemsAnimation(){
    for(int i = 0;i < NUMBER_OF_ITEMS;i++){
        Item *item = GetItemById(i);

        UpdateAnimation(&item->sprite.animation);

    }

}