#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>
#include <inventory.h>
#include <object.h>
#include <npc.h>

typedef struct Player
{
    Sprite sprite;
    Object object;
    Inventory inventory;

}Player;


//Função para mover o player
void MovePlayer(Player *self);

//Função para desenhar o player na tela
void DrawPlayer(Player *self);

//Função para setar a animação do player
void SetPlayerAnimation(animation *self,state state);

//Função que muda a animação do player de acordo com seu estado
void PlayerStatemachine(Player *self);

//Função de interagir com o Npc
void InteractWithNpc(Npc *npcList,GameManager *gameManager);

#endif