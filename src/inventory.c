#include "inventory.h"

#include <string.h>

void InitInventory(Inventory *self) {
    self->numberOfOcuppiedSlots = 0;

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        self->slots[i].itemId = -1;
        self->slots[i].quantity = 0;
    }
}

bool AddItemToInventory(Inventory *self, ItemEntity itemEntity, int quantity){
    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        if(self->slots[i].itemId == itemEntity.itemId && self->slots[i].quantity < MAX_ITEMS){
            self->slots[i].quantity += quantity;
            return true;
        }
    }

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        if (self->slots[i].itemId == -1){
            self->slots[i].itemId = itemEntity.itemId;
            self->slots[i].quantity = quantity;
            self->numberOfOcuppiedSlots++;
            return true;
        }
    }

    return false;
}

void DrawInventory(Inventory *self, Vector2 position) {
    
    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        float x = position.x + (i % (NUMBER_OF_SLOTS/2)) * 50;
        float y = position.y + (i / (NUMBER_OF_SLOTS/2)) * 50;

        DrawRectangleLines(x, y, 48, 48, GRAY);

        if (self->slots[i].itemId != -1) {
            Item *item = GetItemById(self->slots[i].itemId);

            Rectangle src = GetAnimationFrame(item->sprite,(Vector2){6,1});
            Rectangle dest = (Rectangle){x+1,y+8,32,32};

            DrawTexturePro(
                item->sprite.texture,
                src,
                dest,
                (Vector2){0,0},
                0,
                WHITE
            );

            DrawText(TextFormat("x%d",self->slots[i].quantity),x + 28, y + 32, 10, WHITE);
        }
    }
}
