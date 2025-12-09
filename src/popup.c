#include "popup.h"

#include <raylib.h>
#include <game.h>
#include <stdlib.h>
#include <item.h>
#include <string.h>
#include <utils.h>

Popup CreatePopup(char *content,float speed){

    float posX = WINDOW_WIDTH - (MeasureText(content,FONT_SIZE) + 20);
    float posY = WINDOW_HEIGHT - (FONT_SIZE + 10);

    Popup self = (Popup){.alpha = 1.0f,.speed=speed,.position = (Vector2){posX,posY}};

    strncpy(self.content, content, sizeof(self.content));

    return self;
}

void DeletePopup(Popup **self){
    free(*self);
    (*self) = NULL;
}

void UpdatePopup(Popup **self){
    if(!(*self)) return;

    (*self)->position.y -= (*self)->speed;

    (*self)->alpha = lerp((*self)->alpha,0,0.01);

    if((*self)->alpha <= 0.01f){
        DeletePopup(self);
    }

}

void Drawpopup(Popup *self){
    if(!self) return;

    Color textColor = ColorAlpha(WHITE,self->alpha);

    DrawText(self->content,self->position.x,self->position.y,FONT_SIZE,textColor);

}

void PreDoneCollectItemPopup(int collectedItemId,Popup **self){
    char message[128];
    Item *item = GetItemById(collectedItemId);

    Popup *popup = malloc(sizeof(Popup)); 

    snprintf(message, sizeof(message), "Coletou:%s", item ? item->nome : "???");

    *popup = CreatePopup(message,0.5);

    *self = popup;

}

