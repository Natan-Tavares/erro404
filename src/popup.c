#include "popup.h"

#include <raylib.h>
#include <game.h>
#include <stdlib.h>
#include <item.h>
#include <string.h>
#include <utils.h>

Popup CreatePopup(char *content,float moveSpeed){

    int fontSize = 25;

    float posX = WINDOW_WIDTH - (MeasureText(content,fontSize) + 20);
    float posY = WINDOW_HEIGHT - (fontSize + 10);

    Popup self = (Popup){.alpha = 1.0f,.moveSpeed=moveSpeed,.fontSize=fontSize,.position = (Vector2){posX,posY}};

    strncpy(self.content, content, sizeof(self.content));

    return self;
}

Popup CreatePopupPro(char *content,float moveSpeed,int fontSize,Vector2 position,float initialAlpha){
    Popup self = (Popup){.alpha=initialAlpha,.fontSize=fontSize,.moveSpeed=moveSpeed,.position=position};

    strncpy(self.content,content,sizeof(self.content));

    return self;

}

void DeletePopup(Popup **self){
    free(*self);
    (*self) = NULL;
}

void UpdatePopup(Popup **self){
    if(!(*self)) return;

    (*self)->position.y -= (*self)->moveSpeed;

    (*self)->alpha = lerp((*self)->alpha,0,0.01);

    if((*self)->alpha <= 0.01f){
        DeletePopup(self);
    }

}

void Drawpopup(Popup *self){
    if(!self) return;

    Color textColor = ColorAlpha(WHITE,self->alpha);

    DrawText(self->content,self->position.x,self->position.y,self->fontSize,textColor);

}

void PreDoneCollectItemPopup(int collectedItemId,Popup **self){
    char message[128];
    Item *item = GetItemById(collectedItemId);

    Popup *popup = malloc(sizeof(Popup)); 

    snprintf(message, sizeof(message), "Coletou:%s", item ? item->nome : "???");

    *popup = CreatePopup(message,0.5);

    *self = popup;

}

void PreDoneAcceptQuestPopUp(char *message,Popup **self){
    Popup *popup = malloc(sizeof(Popup));

    int fontSize = 30;

    float posX = (WINDOW_WIDTH/2) - (MeasureText(message,fontSize)/2);
    float posY = (WINDOW_HEIGHT/2) - (fontSize/2);

    *popup = CreatePopupPro(message,0,fontSize,(Vector2){posX,posY},1);

    *self = popup;

}
