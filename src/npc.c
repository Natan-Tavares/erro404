#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <player.h>
#include <inventory.h>
#include <utils.h>

Npc *GetNpcCatalog(){
    static Npc catalog[NUMBER_OF_NPCS];
    static bool isInitialized = false;
    
    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Npc){.id=0,.name="!!!!@$$W!(YR&*(U&R))",.type=QUEST_GIVER,.questId=0,.sprite = (Sprite){.texture=LoadTexture("resources/textures/the_thing.png")}};

        isInitialized = true;
    }

    return catalog;
}

void FreeNpcCatalog(){
    Npc *catalog = GetNpcCatalog();

    for(int i = 0;i< NUMBER_OF_NPCS;i++){
        if(catalog[i].sprite.texture.id != 0){
            UnloadTexture(catalog[i].sprite.texture);
            catalog[i].sprite.texture.id = 0;
        }
    }
}

Npc *GetNpcById(int id){
    Npc *catalog = GetNpcCatalog();

    for(int i =0;i < NUMBER_OF_NPCS;i++){
        if(catalog[i].id == id) return &catalog[i];
    }

    return NULL; 
}

/*
    conta o numero de npcs em um arquivo, passado o arquivo para leitura
    le cada linha e se ela começar com um # soma +1 a quantidade de npcs.
    Nota: cada # é um npc é possivel se escrever coisas dps do # mas mantenha
    um # por npc.
*/
int CountNpcEntitys(FILE *file){

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line,sizeof(line),file))
    {

        if(line[0] == '#') count++;
    }
    
    rewind(file);
    return count;

}

void HandleId(NpcEntity *npcEntity,const char *value){
    npcEntity->npcId = atoi(value);
}

void HandlePosition(NpcEntity *npc,const char *value){
    sscanf(value,"%f,%f",&npc->position.x,&npc->position.y);
}

void HandleDialogue(NpcEntity *npcEntity,const char *value){
    strncpy(npcEntity->normalDialogue.text,value,MAX_DIALOGUE_LENGTH -1);
    replaceEscapedNewlines(npcEntity->normalDialogue.text);
}

void HandleRequestDialogue(NpcEntity *npcEntity,const char *value){
    strncpy(npcEntity->requestDialogue.text,value, MAX_DIALOGUE_LENGTH-1);
    replaceEscapedNewlines(npcEntity->requestDialogue.text);
}

void HandleThanksDialogue(NpcEntity *npcEntity,const char *value){
    strncpy(npcEntity->thanksDialogue.text,value, MAX_DIALOGUE_LENGTH-1);
    replaceEscapedNewlines(npcEntity->thanksDialogue.text);
}

/*
    Função que preenche os valores de um npc.
    recebendo um ponteiro para um npc e uma string,
    analisa a string procurando pelos dados especificos do npc
    Então pega os dados e preenche os campos do npc.
*/
void fillNpcEntityValues(NpcEntity *NpcEntity,char *line){

    static const NpcFieldHandler npcHandlers[] = {
        {"id:", HandleId},
        {"position:", HandlePosition},
        {"dialogue:", HandleDialogue},
        {"requestDialogue:",HandleRequestDialogue},
        {"thanksDialogue:",HandleThanksDialogue}
    };

    static const int npcHandleCount = sizeof(npcHandlers) / sizeof(npcHandlers[0]);

    for(int i = 0; i<npcHandleCount; i++){
        if(!strncmp(line, npcHandlers[i].key , strlen(npcHandlers[i].key) )){
            npcHandlers[i].handle(NpcEntity,line + strlen(npcHandlers[i].key));
        }
    }

}

/*
    Função para ler um arquivo com as informações dos npcs
    le linha por linha do arquivo passando cada linha para a função
    de preencher os valos do npc, checando se tem um # antes de cada 
    declaração de npc,aumenta o indice da lista  
*/
void ReadNpcs(NpcEntity *npcEntityList,FILE *file){

    char line[MAX_LINE_LENGTH];
    int index = -1;

    while(fgets(line,sizeof(line),file))
    {
        if(strlen(line) == 0) continue; //checa se a linha esta vazia

        if(line[0] == '#'){ //Checa se encontrou um npc novo
            index++; //se tiver um # aumenta o indice da lista de npcs
            memset(&npcEntityList[index], 0, sizeof(NpcEntity));          
            continue;  
        } 

        line[strcspn(line, "\n")] = '\0'; //retira o ultimo \n da string da linha

        fillNpcEntityValues(&npcEntityList[index],line); //preenche os valores do npc de acordo com a linha passada

    }

}

/*
    Função para carregar a lista de npcs no arquivo de npcs
    Recebendo o nome do arquivo que contem as caracteristicas do npcs
    abre o arquivo, checa se ele é manipulavel, conta o numero de npcs no arquivo
    malloca o numero de npcs para uma lista de npcs, preenche essa lista de npcs
    com a função ReadNpcs e então retorna a lista.
    *Nota: Lembre-se de dar free na lista de npcs no final do programa
*/
NpcEntity *LoadNpcs(const char* filename,int *numberOfNpcs){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfNpcs = CountNpcEntitys(file); //conta o numero de npcs

    NpcEntity *npcs = malloc(sizeof(NpcEntity) * (*numberOfNpcs));

    ReadNpcs(npcs,file); //le o arquivo preenchendo os npcs  na lista

    fclose(file);
    return npcs;
}


/*
    Função para Desenhar os npcs no mapa.
    Recebendo a lista de npcs que deseja desenhar e o numero de npcs,
    checa a variavel isPlayerNearby para saber se o player esta proximo
    e muda a sua cor de acordo com a proximidade do player;s
*/
void DrawNpcs(NpcEntity *npcEntityList,int numberOfNpcs){

    for(int i = 0;i < numberOfNpcs;i++){
        Npc *npc = GetNpcById(npcEntityList[i].npcId);

        if (!npc) continue;

        DrawTexturePro(
            npc->sprite.texture,
            (Rectangle){.height=16,.width=16,.x=0,.y=0},
            (Rectangle){.height=16,.width=16,.x=npcEntityList[i].position.x,.y=npcEntityList[i].position.y},
            (Vector2){0,0},
            0.0,
            WHITE
        );

    }
}

/*
    Função para atualizar o valor do isPlayerNearby dos npcs
    Recebendo o npc para checar,o sprite do plyer e uma distancia de detecção
    Calcula a distancia do npc para o player e checa se ele é menor que o raio
    de detecção
*/
  void UpdateNpcProximity(NpcEntity *npcEntity,ObjectEntity object, float detectionRange) {
    float dist = GetDistance(npcEntity->position, object.position);
    npcEntity->isPlayerNearby = (dist <= detectionRange);

}

 /*
     Atualiza a poximidade de todos os npcs
     Recebendo a lista de npcs, passa npc por npc para a função UpdateNpcProximity
*/
void CheckAllNpcProximities(NpcEntity *npcEntityList,Player player,GameManager gameManager){
    for (int i = 0; i < gameManager.numberOfNpcEntitys; i++) {
        UpdateNpcProximity(&npcEntityList[i], player.object, 25);
    }
}

/*
     Função para checar se o npc tem dialogos.
     Recebendo o npc e o gerenciador do jogo, checa se o npc passado tem dialogos
     e se tiver chama a função StartNpcDialogue para iniciar o primeiro dialogo. 
*/
void TalkToNpc(Player *player,NpcEntity *npcEntity,GameManager *gameManager){
    if(!npcEntity) return;

    Npc *npc = GetNpcById(npcEntity->npcId);
    Quest *quest = GetQuestById(npc->questId);

    gameManager->activeNpc = npc;

    if(npc->type == QUEST_GIVER && !GetQuestById(npc->questId)->isActive){
        
        switch (quest->status)
        {
        case NOT_STARTED:
            StartDialogue(&npcEntity->requestDialogue,gameManager,CHOICE);
            break;
        case IN_PROGRESS:
            StartDialogue(&npcEntity->normalDialogue,gameManager,NONE);
            break;
        case COMPLETED:
            StartDialogue(&npcEntity->thanksDialogue,gameManager,NONE);
            break;
        }
    }else if(npc->type == QUEST_GIVER && GetQuestById(npc->questId)->isActive){
        if (CheckInventoryHasItem(player->inventory,quest->requiredItemId,quest->numberOfRequiredItem))
        {
            StartDialogue(&npcEntity->normalDialogue,gameManager,GIVE);
        }else{
            StartDialogue(&npcEntity->normalDialogue,gameManager,NONE);
        }
    }else{
        StartDialogue(&npcEntity->normalDialogue,gameManager,NONE);
    }
    
}

void UpdateNpc(Player *player,NpcEntity *npcEntityList,GameManager *gameManager){
    CheckAllNpcProximities(npcEntityList,*player,*gameManager);

    InteractWithNpc(player,npcEntityList,gameManager);

    UpdateDialogue(gameManager);

    UpdateQuestChoice(player,gameManager);    
}