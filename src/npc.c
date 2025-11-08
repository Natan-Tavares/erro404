#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>

/*
    conta o numero de npcs em um arquivo, passado o arquivo para leitura
    le cada linha e se ela começar com um # soma +1 a quantidade de npcs.
    Nota: cada # é um npc é possivel se escrever coisas dps do # mas mantenha
    um # por npc.
*/
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

void replaceEscapedNewlines(char *text) {
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

void fillNpcValues(Npc *npc,char *line){

    if(!strncmp(line,"id:",3)){
        npc->dialogueCount = 0;
        npc->id = atoi(line+3);
        
    }else if(!strncmp(line,"position:",9)){
        sscanf(line+9,"%f,%f",&(npc->position.x),&(npc->position.y));

    }else if(!strncmp(line,"dialogue:",9)){
        const char *dialogueText = line + 9;

        int currentDialogue = npc->dialogueCount;
        strncpy(npc->dialogues[currentDialogue].text,dialogueText, MAX_DIALOGUE_LENGTH - 1);

        npc->dialogues[currentDialogue].text[MAX_DIALOGUE_LENGTH - 1] = '\0';

        replaceEscapedNewlines(npc->dialogues[currentDialogue].text);

        npc->dialogueCount++; 
    }

}

/*

*/
void ReadNpcs(Npc *npcList,FILE *file){

    char line[MAX_LINE_LENGTH];
    int index = -1;

    while(fgets(line,sizeof(line),file))
    {
        if(line[0] == '#'){
          index++;
          continue;  
        } 

        if(strlen(line) == 0) continue; //checa se a linha esta vazia
        line[strcspn(line, "\n")] = '\0'; //retira o ultimo \n da string da linha

        fillNpcValues(&npcList[index],line); //preenche os valores do npc de acordo com a linha passada

    }

}

Npc *LoadNpcs(const char* filename,int *numberOfNpcs){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfNpcs = CountNpcs(file); //conta o numero de npcs

    Npc *npcs = malloc(sizeof(Npc) * (*numberOfNpcs));

    ReadNpcs(npcs,file); //le o arquivo preenchendo os npcs  na lista

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

extern float GetDistance(Vector2 a,Vector2 b);

void UpdateNpcProximity(Npc *npc,Sprite player, float detectionRange) {
    float dist = GetDistance(npc->position, player.position);
    npc->isPlayerNearby = (dist <= detectionRange);

}

void CheckAllNpcProximities(Npc *npcList, Sprite player,GameManager gameManager){
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