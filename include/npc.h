
#ifndef NPC_H
#define NPC_H

#include <raylib.h>
#include <animation.h>

#define MAX_LINE_LENGTH 512
#define MAX_DIALOGUE 10
#define MAX_DIALOGUE_LENGTH 512

typedef struct 
{
    int id;
    animation animation;
    Vector2 position;
    char dialogues[MAX_DIALOGUE][MAX_DIALOGUE_LENGTH];
    int dialogueCount;

}npc;

npc *LoadNpcs(const char* filename,int *numberOfNpcs);

void DrawNpcs(npc *npcList,int numberOfNpcs);

#endif