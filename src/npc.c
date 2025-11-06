#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>

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
        line[strcspn(line, "\r\n")] = '\0';

        if(line[0] == '#' || strlen(line) == 0) continue;

        line[strcspn(line, "\n")] = '\0';

        if(!strncmp(line,"id:",3)){
            index++;
            npcs[index].dialogueCount = 0;
            npcs[index].id = atoi(line+3);

        }else if(!strncmp(line,"position:",9)){
            sscanf(line+9,"%f,%f",&npcs[index].position.x,&npcs[index].position.y);

        }else if(!strncmp(line,"dialogue:",9)){
            const char *dialogueText = line + 9;
            if (dialogueText[0] == '\0') continue;

            int currentDialogue = npcs[index].dialogueCount;
            strncpy(npcs[index].dialogues[currentDialogue].text,dialogueText, MAX_DIALOGUE_LENGTH - 1);

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

void DrawNpcs(Npc *npcList,GameManager gameManager){

    for(int i = 0;i < gameManager.numberOfNpcs;i++){
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

extern float GetDistance(Vector2 a,Vector2 b);

void UpdateNpcProximity(Npc *npc,Sprite player, float detectionRange) {
    float dist = GetDistance(npc->position, player.position);
    npc->isPlayerNearby = (dist <= detectionRange);

}

void CheckNpcProximities(Npc *npcList, Sprite player,GameManager gameManager){
    for (int i = 0; i < gameManager.numberOfNpcs; i++) {
        UpdateNpcProximity(&npcList[i], player, 60);

    }
}

void StartNpcDilogue(Npc *npc,int dialogueIndex,GameManager *gameManager){
    if(!npc) return;

    gameManager->activeNpc = npc;
    gameManager->activeDialogueindex = dialogueIndex;
    gameManager->dialogueDelay = 0.2f;
    gameManager->justPressedInteract = true;

    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);
    activeDialogue->visibleChars = 0;
    activeDialogue->activate = true;
    

}

void TalkToNpc(Npc *npc,GameManager *game){
    if(!npc) return;

    if (npc->dialogueCount > 0){
        StartNpcDilogue(npc,0,game);
    }
    
}

void StopNpcDialogue(GameManager *gameManager){
    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);

    activeDialogue->activate = false;
    activeDialogue->visibleChars = 0;

    gameManager->activeDialogueindex = 0;
    gameManager->activeNpc = NULL;

}

void UpdateActiveDialogue(GameManager *game){
    if(!game->activeNpc) return;

    static float timer = 0;
    float lettersDelay = 0.5;

    if(game->justPressedInteract){
        game->justPressedInteract = false;
        return;
    }

    Dialogue *activeDialogue = &(game->activeNpc->dialogues[game->activeDialogueindex]);
    
    if(!activeDialogue->activate){
        game->activeNpc = NULL; 
        return;
    } 

    int textLen = (int)strlen(activeDialogue->text);
    timer += GetFrameTime();

    if(timer >= lettersDelay && activeDialogue->visibleChars < textLen){
        activeDialogue->visibleChars++;
    }

    if(activeDialogue->visibleChars >= textLen && IsKeyPressed(KEY_E)){
        if(game->activeDialogueindex + 1 < game->activeNpc->dialogueCount){

            activeDialogue->activate = false;
            StartNpcDilogue(game->activeNpc,game->activeDialogueindex +1,game);
            return;
        } else {
            StopNpcDialogue(game);
            return;
        }
    }

    if(activeDialogue->visibleChars < textLen && IsKeyPressed(KEY_E) && !game->justPressedInteract){
        activeDialogue->visibleChars = textLen;
    }
}

void DrawActiveDialogue(GameManager *game){
    
    if(!game->activeNpc) return;

    Dialogue *activeDialogue = &(game->activeNpc->dialogues[game->activeDialogueindex]);
    if( !activeDialogue->activate) return;

    DrawRectangle(50,400,700,150,Fade(BLACK,0.8));

    char buffer[MAX_DIALOGUE_LENGTH];
    strncpy(buffer,activeDialogue->text,activeDialogue->visibleChars);
    buffer[activeDialogue->visibleChars] = '\0';

    DrawText(buffer,70, 420, 20, WHITE);

}