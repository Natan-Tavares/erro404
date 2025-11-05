#include "npc.h"

#include <tilemap.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>

/*
    Função para checar se foi possivel manipular um arquivo.
*/
extern bool CheckFile(FILE *file);

int CountNpcs(FILE *file){

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line,sizeof(line),file))
    {

        if(line[0] == '#') count++;
    }
    
    rewind(file);
    return count;

}

void ReadNpcs(npc *npcs,FILE *file){

    char line[MAX_LINE_LENGTH];

    int index = -1;
    while(fgets(line,sizeof(line),file))
    {

        if(line[0] == '#' || strlen(line) == 0) continue;

        line[strcspn(line, "\n")] = '\0';

        if(!strncmp(line,"id:",3)){
            index++;
            npcs[index].dialogueCount = 0;
            npcs[index].id = atoi(line+3);

        }else if(!strncmp(line,"position:",9)){
            sscanf(line+9,"%f,%f",&npcs[index].position.x,&npcs[index].position.y);

        }else if(!strncmp(line,"dialogue:",9)){
            int dialogue = npcs[index].dialogueCount;
            strncpy(npcs[index].dialogues[dialogue],line + 9, MAX_DIALOGUE_LENGTH - 1);

            npcs[index].dialogues[dialogue][MAX_DIALOGUE_LENGTH - 1] = '\0';
            npcs[index].dialogueCount++; 
        }

    }

}

npc *LoadNpcs(const char* filename,int *numberOfNpcs){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfNpcs = CountNpcs(file);

    npc *npcs = malloc(sizeof(npc) * (*numberOfNpcs));

    ReadNpcs(npcs,file);

    fclose(file);
    return npcs;
}

void DrawNpcs(npc *npcList,int numberOfNpcs){

    for(int i = 0;i < numberOfNpcs;i++){

        DrawRectangle(
            npcList[i].position.x,
            npcList[i].position.y,
            TILE_SIZE,
            TILE_SIZE,
            GREEN
        );
    }
}