#ifndef CHOICE_H
#define CHOICE_H

#include <raylib.h>

#define MAX_DESCRIPTION_LENGTH 128

typedef void (*OptionCallback)(void *context);

typedef struct ChoiceOption
{
    char text[64];
    int option;

    OptionCallback callback;
    void * callbackContext;

}ChoiceOption;

typedef struct ChoiceMenu
{
    char description[MAX_DESCRIPTION_LENGTH];

    ChoiceOption options[10];

    int optionAmount;
    int selectedIndex;

    bool active;
    bool justOpened;
}ChoiceMenu;

ChoiceMenu CreateChoiceMenu();

void FillCallbacks(ChoiceMenu *self, ...);

void UpdateChoiceMenu(ChoiceMenu *self);

void DrawChoiceMenu(ChoiceMenu self);

#endif