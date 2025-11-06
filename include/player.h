#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>
#include <npc.h>

//Função para mover o player
void MovePlayer(Sprite *self);

//Função para desenhar o player na tela
void DrawPlayer(Sprite *self,Texture2D textura);

//Função para setar a animação do player
void SetPlayerAnimation(animation *self,state state);

//Função que muda a animação do player de acordo com seu estado
void PlayerStatemachine(Sprite *self);

//Função de interagir com o Npc
void InteractWithNpc(Npc *npcList,GameManager *gameManager);

#endif