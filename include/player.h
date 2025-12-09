#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>
#include <inventory.h>
#include <object.h>

typedef struct Player
{
    Sprite sprite;
    ObjectEntity object;
    Inventory inventory;

    bool canInteract;

}Player;

typedef struct NpcEntity NpcEntity; 

//Função para mover o player
void MovePlayer(Player *self);

//Função para desenhar o player na tela
void DrawPlayer(Player *self);

//Função para setar a animação do player
void SetPlayerAnimation(animation *self,state state);

//Função que muda a animação do player de acordo com seu estado
void PlayerStatemachine(Player *self);

//Função de interagir com o Npc
void InteractWithNpc(Player *player,NpcEntity *npcEntityList,GameManager *gameManager);

void UpdatePlayer(Player *player,ObjectEntity *objectEntityList,unsigned char *map,GameManager game);

#endif