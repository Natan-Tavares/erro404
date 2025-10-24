
#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>

//Definições de estados
typedef enum state{

    IDLE = 0,
    WALK = 1

}state;

//animação, 6 componentes
typedef struct animation
{
    int first; //index do primeiro frame
    int last; //index do ultimo frame

    int current; //index do frame atual
    float speed; //tempo que leva ate o proximo frame(quanto menor mais rapido)

    float durationLeft; //duração ate o proximo frame(inicialize = speed)

    state state; // animação em execução

}animation;

//Função para atualizar a animação
void UpdateAnimation(animation *animation);

//Função para pegar o espaço no arquivo do frame
Rectangle GetAnimationFrame(animation *animation,int numframesPerRow);

#endif
