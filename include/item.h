#ifndef ITEM_H
#define ITEM_H

#include <raylib.h>
#include <sprite.h>

#define NUMBER_OF_ITEMS 6 //numero de itens que o jogo tem

typedef struct GameManager GameManager; //Definindo a struct do gerenciador do jogo que foi declarada em outro scopo 
typedef struct Player Player;

typedef struct Item 
{
    int id;
    char nome[64];
    Sprite sprite;
}Item;

typedef struct ItemEntity
{
    int itemId;
    Vector2 position;
    bool colected;

}ItemEntity;

typedef void (*ItemValueHandler)(ItemEntity *itemEntity, const char *value);

typedef struct ItemFieldHandler
{
    const char *key;
    ItemValueHandler handle;
    
}ItemFieldHandler;

Item *GetItemById(int id);

ItemEntity *LoadItems(const char* filename,int *numberOfItems);

void FreeItemCatalog();

Item *GetItemCatalog();

void DrawItemEntityList(ItemEntity *ItemEntityList,int numberOfItemEntitys);

void UpdateItemEntity(ItemEntity *ItemEntityList,GameManager *gameManager,Player *player);

void UpdateItemsAnimation();

#endif