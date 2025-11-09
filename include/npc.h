
#ifndef NPC_H
#define NPC_H

#include <raylib.h>
#include <sprite.h>
#include <animation.h>

#define MAX_LINE_LENGTH 512 //Numero maximo de caracteres numa linha de arquivo de npcs
#define MAX_DIALOGUE 10 //Numero maximo de dialogos que um npc pode ter
#define MAX_DIALOGUE_LENGTH 512 //Numero maximo de caracteres que um dialogo de npc pode ter

typedef struct GameManager GameManager; //Definindo a struct do gerenciador do jogo que foi declarada em outro scopo 
typedef struct Player Player;

//Dialogo,3 componentes
typedef struct 
{

    char text[MAX_DIALOGUE_LENGTH]; //Componente que armazena o texto do dialogo
    int visibleChars; // Componente que armazena a quantidade de caracteres visiveis do dialogo
    bool activate; //Componente que armazena se o dialogo esta ativo ou não
    
}Dialogue;

//Npc,6 componentes
typedef struct 
{
    int id; //componente que armazena o id no npc
    animation animation; //Componente que armazena animação do npc
    Vector2 position; //Componente que armazena a posição do npc
    Dialogue dialogues[MAX_DIALOGUE]; //Componente que armazena os dialogos do npc
    int dialogueCount; //Componente que armazena o numero que dialogos que o npc tem
    bool isPlayerNearby; //Componente que armazena se o player esta proximo ou não do npc

}Npc;

//Carrega os npcs dentro do arquivo passado, armazenando o numero de npcs no inteiro passado retornando a lista de npcs
Npc *LoadNpcs(const char* filename,int *numberOfNpcs);

//Desenha os npcs da lista passada 
void DrawNpcs(Npc *npcList,int numberOfNpcs);

//Checa as proximidades de todos os npcs na lista passada na função
void CheckAllNpcProximities(Npc *npcList,Player player,GameManager gameManager);

//Função para checar se o npc tem dialogos
void TalkToNpc(Npc *npc,GameManager *game);

//Função para atualizar os dialogos do npc ativo
void UpdateActiveDialogue(GameManager *game);

//Função para desenhar os dialogos do npc ativo
void DrawActiveDialogue(GameManager *game);

#endif