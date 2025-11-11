#ifndef ULTILS_H
#define ULTILS_H

#include <raylib.h>
#include <game.h>
#include <stdio.h>

void replaceEscapedNewlines(char *text);

    //Função para fazer interpolação linear de dois valores
float lerp(float start, float end, float t);

    //Função para pegar a menor distancia entre dois vetores
float GetDistance(Vector2 a,Vector2 b);

    //Função para atualizar os caracteres visivies de uma string
void UpdateVisibleChars(char *text,int *visibleChars,float lettersDelay);

    //Função para checar se um arquivo é valido
bool CheckFile(FILE *file);

    //Transforma um valor false em true
void UpdateBoolValue(bool *value);

#endif