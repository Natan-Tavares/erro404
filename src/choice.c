#include <choice.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

ChoiceMenu CreateChoiceMenu(){
    ChoiceMenu choiceMenu = {0};
    choiceMenu.optionAmount = 2;
    
    ChoiceOption options[2] = {(ChoiceOption){.text="Sim",.option=0},(ChoiceOption){.text="Não",.option=1}}; 

    for(int i = 0;i < choiceMenu.optionAmount;i++){
        choiceMenu.options[i] = options[i];
    }

    return choiceMenu;
}

void FillCallbacks(ChoiceMenu *self, ...){
    va_list args;
    va_start(args,self);

    for(int i = 0;i < self->optionAmount;i++){
        OptionCallback callback = va_arg(args, OptionCallback);
        void * context = va_arg(args, void *);


        self->options[i].callback = callback;
        self->options[i].callbackContext = context;
    }
    va_end(args);
}

void HandleInput(ChoiceMenu *self){

    if(IsKeyPressed(KEY_UP)) self->selectedIndex = (self->selectedIndex+1) % self->optionAmount; 
    if(IsKeyPressed(KEY_DOWN)) self->selectedIndex = (self->selectedIndex+1) % self->optionAmount;

}

void UpdateChoiceMenu(ChoiceMenu *self){
    if(!self->active) return;

    HandleInput(self);

    if(self->justOpened){
        self->justOpened = false;
        return;
    }

    if(IsKeyPressed(KEY_E)){
        for(int i = 0;i < self->optionAmount;i++){
            if(self->options[i].option == self->selectedIndex){
                self->options[i].callback(self->options[i].callbackContext);
                break;
            }
        }

        self->active = false;
    }
}

void DrawChoiceMenu(ChoiceMenu self){
    if(!self.active) return;
    
    DrawRectangle(100,600,400,120,Fade(BLACK,0.7));
    DrawText(self.description,120,620,20,WHITE);

    for(int i = 0;i < 2;i++){
        Color color = (i == self.selectedIndex ) ? YELLOW : WHITE;
        DrawText(self.options[i].text,140,650 + i * 25,20,color);
    }

}