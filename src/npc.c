#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <npc.h>
#include <string.h>
#include <quest.h>
#include <raylib.h>
#include <choice.h>
#include <player.h>
#include <inventory.h>
#include <utils.h>

Npc *GetNpcCatalog(){
    static Npc catalog[NUMBER_OF_NPCS];
    static bool isInitialized = false;
    
    if(!isInitialized){
        memset(catalog,0,sizeof(catalog));
        catalog[0] = (Npc){.id=0,.name="!!!!@$$W!(YR&*(U&R))",.type=QUEST_GIVER,.questId=0,
            .sprite = (Sprite){.texture=LoadTexture("resources/textures/the_thing.png"),
            .animation = (animation){.numFramesPerAxle=(Vector2){2,1},.first = 0 ,.last = 2,.durationLeft = 0.1,.speed=0.5,.state = IDLE}
            }};
        catalog[1] = (Npc){.id=1,.name="criança",.type=QUEST_GIVER,.questId=1,
            .sprite = (Sprite){.texture=LoadTexture("resources/textures/crianca.png"),
            .animation = (animation){.numFramesPerAxle=(Vector2){1,1},.first = 0,.last = 1,.durationLeft = 0.1,.speed=0.5,.state = IDLE}
            }};
        catalog[2] = (Npc){.id=2,.name="O GUIA",.type=NORMAL,
            .sprite = (Sprite){.texture=LoadTexture("resources/textures/guia.png"),
            .animation = (animation){.numFramesPerAxle={11,1},.first=0,.last=11,.speed=0.25,.state=IDLE}
            }};
        catalog[3] = (Npc){.id=3,.name="Placa",.type=NORMAL,
            .sprite = (Sprite){.texture=LoadTexture("resources/textures/placa.png"),
            .animation = (animation){.numFramesPerAxle={1,1},.first=0,.last=0,.state=IDLE}
            }};

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

void HandleNpcId(NpcEntity *npcEntity,const char *value){
    npcEntity->npcId = atoi(value);

    Npc *npc = GetNpcById(npcEntity->npcId); 

    npcEntity->normalDialogue = npc->type == QUEST_GIVER ? CreateDialogueAs(AWAITING_ITEM) : CreateDialogueAs(NORMAL);
    npcEntity->requestDialogue = CreateDialogueAs(ASK);
    npcEntity->thanksDialogue = CreateDialogueAs(THANKS);

}

void HandleNpcPosition(NpcEntity *npc,const char *value){
    sscanf(value,"%f,%f",&npc->position.x,&npc->position.y);
}

void HandleNpcDialogue(NpcEntity *npcEntity,const char *value){
    Dialogue *dialogue = &npcEntity->normalDialogue;

    FillDialogue(dialogue,value);
}

void HandleNpcRequestDialogue(NpcEntity *npcEntity,const char *value){
    Dialogue *dialogue = &npcEntity->requestDialogue;

    FillDialogue(dialogue,value);
}

void HandleNpcThanksDialogue(NpcEntity *npcEntity,const char *value){
    Dialogue *dialogue = &npcEntity->thanksDialogue;

    FillDialogue(dialogue,value);
}

/*
    Função que preenche os valores de um npc.
    recebendo um ponteiro para um npc e uma string,
    analisa a string procurando pelos dados especificos do npc
    Então pega os dados e preenche os campos do npc.
*/
void fillNpcEntityValues(NpcEntity *NpcEntity,char *line){

    static const NpcFieldHandler npcHandlers[] = {
        {"id:", HandleNpcId},
        {"position:", HandleNpcPosition},
        {"dialogue:", HandleNpcDialogue},
        {"requestDialogue:",HandleNpcRequestDialogue},
        {"thanksDialogue:",HandleNpcThanksDialogue}
    };

    static const int npcHandleCount = sizeof(npcHandlers) / sizeof(npcHandlers[0]);

    for(int i = 0; i<npcHandleCount; i++){
        if(!strncmp(line, npcHandlers[i].key , strlen(npcHandlers[i].key) )){
            npcHandlers[i].handle(NpcEntity,line + strlen(npcHandlers[i].key));
            return;
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

void UpdateNpcAnimation(){
    for(int i = 0;i < NUMBER_OF_NPCS;i++){
        Npc *npc = GetNpcById(i);
        if(npc) UpdateAnimation(&npc->sprite.animation);
    }

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

        Rectangle animationFrame = GetAnimationFrame(npc->sprite,npc->sprite.animation.numFramesPerAxle);

        DrawTexturePro(
            npc->sprite.texture,
            animationFrame,
            (Rectangle){.height=16,.width=16,.x=npcEntityList[i].position.x,.y=npcEntityList[i].position.y},
            (Vector2){0,0},
            0.0,
            WHITE
        );

    }
}

void DrawNpcDialogue(void *context){
    GameManager *gameManager = (GameManager *)context;

    TextLine activeTextLine = gameManager->activeDialogue->textlines[gameManager->activeDialogue->activeTextLineIndex]; 

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int boxWidth = screenWidth - 200;
    int boxHeight = 200;
    int boxX = 100;
    int boxY = screenHeight - boxHeight - 50;

    DrawRectangleRounded((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(BLACK, 0.7f));
    DrawRectangleRoundedLines((Rectangle){boxX, boxY, boxWidth, boxHeight}, 0.1f, 8, Fade(WHITE, 0.3f));

    Npc *npc = GetNpcById(gameManager->activeNpc->npcId);
 
    if (npc->name && strlen(npc->name) > 0) {
        DrawText(npc->name, boxX + 20, boxY + 10, 24, YELLOW);
    }

    const char *visibleText = TextSubtext(activeTextLine.content, 0, activeTextLine.visibleChars);
    DrawTextEx(GetFontDefault(), visibleText, 
            (Vector2){boxX + 20, boxY + 50}, 20, 2, WHITE);


    int textLen = strlen(activeTextLine.content);
    if (activeTextLine.visibleChars >= textLen) {
        DrawText("Pressione [E] para continuar", boxX + boxWidth - 350, boxY + boxHeight - 30, 18, GRAY);
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

void InteractWithNpc(Player *player,NpcEntity *npcEntityList,GameManager *gameManager){

    for(int i = 0;i < gameManager->numberOfNpcEntitys;i++){

        if(npcEntityList[i].isPlayerNearby && IsKeyPressed(KEY_E) && !gameManager->choiceMenu->active){
            TalkToNpc(player,&npcEntityList[i],gameManager);
        }
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

    gameManager->activeNpc = npcEntity;

    if(npc->type == QUEST_GIVER){
        gameManager->activeQuestId = npc->questId;

        switch (quest->status)
        {
        case NOT_STARTED:
            gameManager->activeDialogue = &npcEntity->requestDialogue;
            break;
        
        case IN_PROGRESS:
            gameManager->activeDialogue = &npcEntity->normalDialogue;
            break;

        case COMPLETED:
            gameManager->activeDialogue = &npcEntity->thanksDialogue;
            break;
        }

    }else{
        gameManager->activeDialogue = &npcEntity->normalDialogue;
    }
    
    DefineCallbacks(gameManager->activeDialogue,StartQuestChoice,gameManager,DrawNpcDialogue,gameManager);
    
}

void UpdateNpc(Player *player,NpcEntity *npcEntityList,GameManager *gameManager){
    CheckAllNpcProximities(npcEntityList,*player,*gameManager);

    InteractWithNpc(player,npcEntityList,gameManager);
    
    UpdateNpcAnimation();
}