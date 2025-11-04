#include "animation.h"
#include <raylib.h>    

/*
    Função para atualizar o frame da animação
    pega a duração desdo ultimo frame do jogo subtrae pelo tempo 
    que falta para o proximo frame da animação e quando chegar a 
    0, passa para o proximo frame ate resetar os frames.
*/
void UpdateAnimation(animation *animation){
    float durationTime = GetFrameTime();
    animation->durationLeft -= durationTime;

    if(animation->durationLeft <= 0.0){
        animation->durationLeft = animation->speed;
        animation->current++;

        if(animation->current >= animation->last){
            animation->current = animation->first;
        }
    }

}


/*
    Função para pegar o frame da animação;
    A posição x no arquivo da animação é pego de acodo com o frame atual
    retornando assim um retangulo com as posições exatas de cada frame no arquivo
    *Nota: os tamanhos do retangulo podem e serão alterados no futuro, pois so são 
    essas devido ao arquivo dos sprites e o seu tamanho.
*/
Rectangle GetAnimationFrame(animation *animation,int numframesPerRow){

    int x = (animation->current % numframesPerRow) * 100;
    int y = (int)animation->state * 100;

    return (Rectangle){
        .x = (float)x,
        .y = (float)y,
        .width = 100,
        .height = 100
    };
}