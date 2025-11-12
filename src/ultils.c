#include "utils.h"

#include <game.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
    Função que repoe \ n dentro de uma string por literais \n.
    com duas variaveis apontando para o espaço de memoria da strig
    as variaveis reescrevem a string de acordo com os caracteres
    reescrevendo os \n.
*/
void replaceEscapedNewlines(char *text){
    char *src = text;
    char *dst = text;

    while (*src) {
        if (src[0] == '\\' && src[1] == 'n') {
            *dst++ = '\n';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }

    *dst = '\0';
}

/*
    Função para aproximar um valor inicial de um final usando um fator
*/
float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

/*
    Função que faz pitagoras entre a distancia x e y de dois vetores para retornar
    a menor distancia entre os dois
*/
float GetDistance(Vector2 a,Vector2 b){

    Vector2 dist = (Vector2){
        .x = (float)abs((a.x - b.x)),
        .y = (float)abs((a.y - b.y))
    };

    return sqrtf((dist.x * dist.x) + (dist.y * dist.y));

}

/*
    Função para checar se foi possivel manipular um arquivo.
*/
bool CheckFile(FILE *file){
    if(file){
        return true;
    }
    perror("Erro No arquivo:");
    return false;
}

/*
    Atualiza os caracteres ativos do dialogo passado de acordo com o delay
    de cada letra.
    *Nota: Faz a animação de letras aparecendo na tela.
*/
void UpdateVisibleChars(char *text,int *visibleChars,float lettersDelay){
    static float timer = 0;
    int textLen = strlen(text);

    timer += GetFrameTime();
    if(timer >= lettersDelay && *visibleChars < textLen){
        (*visibleChars)++;
        timer = 0;

    }
}

/*
    Transforma um valor false em um valor true
*/
void UpdateBoolValue(bool *value){
    if(!*value){
        *value = true;
    }    
}

int FindIntIndex(int searchInt,int *intList,int listSize){
    for(int i = 0;i < listSize;i++){
        if(intList[i] == searchInt){
            return i;
        }
    }
    return -1;
}