#ifndef INVENTORY_H
#define INVENTORY_H

#include <raylib.h>
#include <item.h>

#define MAX_ITEMS 10
#define NUMBER_OF_SLOTS 12

typedef struct Slot
{
    int itemId;
    int quantity;

}Slot;

typedef struct 
{
    Slot slots[NUMBER_OF_SLOTS];
    int numberOfOcuppiedSlots; //numero de slots ocupados

}Inventory;

void InitInventory(Inventory *self);

bool AddItemToInventory(Inventory *self, int itemId, int quantity);

void DrawInventory(Inventory *inv, Vector2 position);

bool CheckInventoryHasItem(Inventory self,int itemId,int quantityOfItem);

void RemoveItem(Inventory *self,int itemId,int quantityToRemove);

#endif
