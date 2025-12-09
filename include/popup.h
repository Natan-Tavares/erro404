#ifndef POPUP_H
#define POPUP_H

#include <raylib.h>

#define MAX_CONTENT_LENGTH 125
#define FONT_SIZE 25

typedef struct Popup
{

char content[MAX_CONTENT_LENGTH];

Vector2 position;

float speed;
float alpha;

}Popup;

Popup CreatePopup(char *content,float speed);

void UpdatePopup(Popup **self);

void Drawpopup(Popup *self);

void PreDoneCollectItemPopup(int collectedItemId,Popup **self);

#endif