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
        float x = position.x + (i % 6) * 50;
        float y = position.y + (i / 6) * 50;

        DrawRectangleLines(x, y, 48, 48, GRAY);

        if (self->slots[i].itemId != -1) {
            DrawText(TextFormat("ID:%d x%d", self->slots[i].itemId, self->slots[i].quantity),
                     x + 5, y + 15, 10, WHITE);
        }
    }
}
