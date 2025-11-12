#include "inventory.h"

#include <string.h>
#include <stdio.h>

void InitInventory(Inventory *self) {
    self->numberOfOcuppiedSlots = 0;

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        self->slots[i].itemId = -1;
        self->slots[i].quantity = 0;
    }
}

bool AddItemToInventory(Inventory *self, int itemId, int quantity){
    if(!quantity) return false;
    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        if(self->slots[i].itemId == itemId && self->slots[i].quantity + quantity < MAX_ITEMS){
            self->slots[i].quantity += quantity;
            return true;
        }else if(self->slots[i].itemId == itemId){
            int overquantity = self->slots[i].quantity + quantity - MAX_ITEMS;
            self->slots[i].quantity = MAX_ITEMS;
            quantity = overquantity;
        }
    }

    for (int i = 0; i < NUMBER_OF_SLOTS; i++) {
        if (self->slots[i].itemId == -1){
            self->slots[i].itemId = itemId;
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

            Rectangle src = GetAnimationFrame(item->sprite,item->sprite.animation.numFramesPerAxle);
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

bool CheckInventoryHasItem(Inventory self,int itemId,int quantityOfItem){
    int count = 0;
    for(int i = 0;i < NUMBER_OF_SLOTS;i++){
        if(self.slots[i].itemId == itemId){
            count += self.slots[i].quantity;
        }
    }
    if(count >= quantityOfItem){
        return true;
    }
    return false;
}

void RemoveItem(Inventory *self,int itemId,int quantityToRemove){
    for(int i = NUMBER_OF_SLOTS-1; i >= 0;i--){
        if(quantityToRemove <= 0) break;
        Slot *slot = &self->slots[i];
        
        if(slot->itemId == itemId){
            if(slot->quantity <= quantityToRemove){
                slot->itemId = -1;
                slot->quantity = 0;
                quantityToRemove -= slot->quantity;
            }else if(slot->quantity > quantityToRemove){
                slot->quantity -= quantityToRemove;
                quantityToRemove = 0;
            }
        }
        
    }
}
