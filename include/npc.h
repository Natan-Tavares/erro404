
#ifndef NPC_H
#define NPC_H

#include <raylib.h>
#include <sprite.h>
#include <animation.h>
#include <dialogue.h>
#include <quest.h>

#define MAX_LINE_LENGTH 512 //Numero maximo de caracteres numa linha de arquivo de npcs
#define MAX_DIALOGUE 10 //Numero maximo de dialogos que um npc pode ter

#define NUMBER_OF_NPCS 4

typedef struct GameManager GameManager; //Definindo a struct do gerenciador do jogo que foi declarada em outro scopo 
typedef struct Player Player;

typedef enum NpcType
{

    NORMAL,
    QUEST_GIVER,

} NpcType;

//Npc,6 componentes
typedef struct Npc 
{
    int id; //componente que armazena o id no npc
    char name[32];
    Sprite sprite;
    animation animation; //Componente que armazena animação do npc

    NpcType type;
    int questId;

}Npc;

typedef struct NpcEntity
{
    int npcId;
    Vector2 position;
    bool isPlayerNearby;

    Dialogue requestDialogue;
    Dialogue thanksDialogue;
    Dialogue normalDialogue;

} NpcEntity;

typedef void (*NpcValueHandler)(NpcEntity *npc, const char *value);

typedef struct NpcFieldHandler
{
    const char *key;
    NpcValueHandler handle;

}NpcFieldHandler;

//Carrega os npcs dentro do arquivo passado, armazenando o numero de npcs no inteiro passado retornando a lista de npcs
NpcEntity *LoadNpcs(const char* filename,int *numberOfNpcs);    

Npc *GetNpcById(int id);

void FreeNpcCatalog();

//Desenha os npcs da lista passada 
void DrawNpcs(NpcEntity *npcEntityList,int numberOfNpcs);

 //Checa as proximidades de todos os npcs na lista passada na função
void CheckAllNpcProximities(NpcEntity *npcEntityList,Player player,GameManager gameManager);

void TalkToNpc(Player *player,NpcEntity *npcEntity,GameManager *gameManager);

void UpdateNpc(Player *player,NpcEntity *npcEntityList,GameManager *gameManager);

#endif