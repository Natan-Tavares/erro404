
#ifndef NPC_H
#define NPC_H

#include <raylib.h>
#include <sprite.h>
#include <animation.h>

#define MAX_LINE_LENGTH 512
#define MAX_DIALOGUE 10
#define MAX_DIALOGUE_LENGTH 512

 typedef struct GameManager GameManager;

typedef struct 
{

    char text[MAX_DIALOGUE_LENGTH];
    int visibleChars;
    bool activate;
    
}Dialogue;

typedef struct 
{
    int id;
    animation animation;
    Vector2 position;
    Dialogue dialogues[MAX_DIALOGUE];
    int dialogueCount;
    bool isPlayerNearby;

}Npc;

Npc *LoadNpcs(const char* filename,int *numberOfNpcs);

void DrawNpcs(Npc *npcList,int numberOfNpcs);

void CheckNpcProximities(Npc *npcList, int numberOfNpcs, Sprite player, float detectionRange);

void TalkToNpc(Npc *npc,GameManager *game);

void UpdateActivateDialogue(GameManager *game);

void DrawActivateDialogue(GameManager *game);

#endif