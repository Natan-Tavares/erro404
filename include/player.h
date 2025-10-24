
#ifndef PLAYER_H
#define PLAYER_H

#include <sprite.h>

//Função para mover o player, não retorna nada
void MovePlayer(Sprite *player);

//Função para desenhar o player na tela
void DrawPlayer(Sprite *player);

//Função para setar a animação do player
void SetPlayerAnimation(Sprite *player,States state);

//Função que muda a animação do player de acordo com seu estado
void PlayerStatemachine(Sprite *player);

#endif