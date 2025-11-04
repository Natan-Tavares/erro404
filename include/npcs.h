
#ifndef NPCS_H
#define NPCS_H

#include <raylib.h>
#include <animation.h>

#define MAX_LINE_LENGTH 512

typedef struct 
{
    int id;

    Vector2 position;

    char dialogues[][];
    int dialogueCount;

    animation animation;

}npc;



#endif