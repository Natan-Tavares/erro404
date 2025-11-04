#include "npcs.h"

#include <stdlib.h>
#include <raylib.h>

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

npc *LoadNpcs(const char* filename){

    FILE file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    char line[MAX_LINE_LENGTH];

    while (fgets(line,sizeof(line),file))
    {

        line[strcspn(line,"\n")] = 0;

        if(line[0] == # || strlen(line) == 0) continue;

    }
    

    fclose(file)
}