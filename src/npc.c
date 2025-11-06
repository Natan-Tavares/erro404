#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <raylib.h>

extern float GetDistance(Vector2 a,Vector2 b);

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

void ReadNpcs(Npc *npcs,FILE *file){

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
            int currentDialogue = npcs[index].dialogueCount;
            strncpy(npcs[index].dialogues[currentDialogue].text,line + 9, MAX_DIALOGUE_LENGTH - 1);

            npcs[index].dialogues[currentDialogue].text[MAX_DIALOGUE_LENGTH - 1] = '\0';
            npcs[index].dialogueCount++; 
        }

    }

}

Npc *LoadNpcs(const char* filename,int *numberOfNpcs){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfNpcs = CountNpcs(file);

    Npc *npcs = malloc(sizeof(Npc) * (*numberOfNpcs));

    ReadNpcs(npcs,file);

    fclose(file);
    return npcs;
}

void DrawNpcs(Npc *npcList,int numberOfNpcs){

    for(int i = 0;i < numberOfNpcs;i++){
        Color color = npcList[i].isPlayerNearby ? YELLOW : GREEN;

        DrawRectangle(
            npcList[i].position.x,
            npcList[i].position.y,
            TILE_SIZE,
            TILE_SIZE,
            color
        );

    }
}

void UpdateNpcProximity(Npc *npc,Sprite player, float detectionRange) {
    float dist = GetDistance(npc->position, player.position);
    npc->isPlayerNearby = (dist <= detectionRange);

}

void CheckNpcProximities(Npc *npcList, int numberOfNpcs, Sprite player, float detectionRange){
    for (int i = 0; i < numberOfNpcs; i++) {
        UpdateNpcProximity(&npcList[i], player, detectionRange);

    }
}

void StartNpcDilogue(Npc *npc,int dialogueIndex,GameManager *game){
    if(!npc) return;

    game->activeNpc = npc;
    game->activeDialogueindex = dialogueIndex;

    Dialogue activateDialogue = game->activeNpc->dialogues[game->activeDialogueindex];
    activateDialogue.visibleChars = 0;
    activateDialogue.activate = true;
}

void StopNpcDialogue(GameManager *game){
    Dialogue activateDialogue = game->activeNpc->dialogues[game->activeDialogueindex];

    activateDialogue.activate = false;
    activateDialogue.visibleChars = 0;

    game->activeDialogueindex = 0;
    game->activeNpc = NULL;

}

void UpdateActivateDialogue(GameManager *game){
    if(!game->activeNpc) return;

    Dialogue *activeDialogue = &game->activeNpc->dialogues[game->activeDialogueindex];
    
    if(!activeDialogue->activate){
        game->activeNpc = NULL;
        return;
    }

    activeDialogue->visibleChars += (int)(GetFrameTime());

    int textLen = (int)strlen(activeDialogue->text);
    if(activeDialogue->visibleChars >= textLen){
        activeDialogue->visibleChars = textLen; 
    }

    if(activeDialogue->visibleChars >= textLen && IsKeyPressed(KEY_E)){
        if(game->activeDialogueindex + 1 < game->activeNpc->dialogueCount){

            activeDialogue->activate = false;
            StartNpcDilogue(game->activeNpc,game->activeDialogueindex +1,game);
        } else {
            StopNpcDialogue(game);
        }
    }

    if(activeDialogue->visibleChars < textLen && IsKeyPressed(KEY_E)){
        activeDialogue->visibleChars = textLen;

    }
}

void TalkToNpc(Npc *npc,GameManager *game){
    if(!npc) return;

    if (npc->dialogueCount > 0){
        StartNpcDilogue(npc,0,game);
    }
    
}

void DrawActivateDialogue(GameManager *game){
    
    if(!game->activeNpc) return;

    Dialogue activeDialogue = game->activeNpc->dialogues[game->activeDialogueindex];
    if( !activeDialogue.activate) return;

    DrawRectangle(50,400,700,150,Fade(BLACK,0.8));

    char buffer[MAX_DIALOGUE_LENGTH];
    strncpy(buffer,activeDialogue.text,activeDialogue.visibleChars);
    buffer[activeDialogue.visibleChars] = '\0';

    DrawText(buffer,70, 420, 20, WHITE);

}