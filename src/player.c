#include <raylib.h>
#include <stdio.h>
#include <sprite.h>
#include "player.h"

//Função para atualizar a velocidade do player multiplando pela speed do sprite
void MovePlayer(Sprite *player){

    float xDirection = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
    float yDirection = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);

    player->velocity.x = xDirection * player->speed;
    player->velocity.y = yDirection * player->speed;

}

/*
Função para mudar o valor da direção em que o player esta virado de acordo com a velocidade
(lembre-se de iniciar a direção com um valor default)
*/
void GetPlayerDirection(Sprite *player){

    if(player->velocity.x > 0){
        player->direction.x = 1;
    }else if(player->velocity.x < 0){
        player->direction.x = -1;
    }

}

/*
Função que para diminuir o tamaho da função de DrawTexturePro 
e virar o player de acordo com a direção em que esta se movendo
(multiplicando o valor do largura do frame que ele desenha pela direção em que esta virado) 
*/
void DrawPlayer(Sprite *player){

    GetPlayerDirection(player);

    DrawTexturePro(
	    player->animation.texture,
		(Rectangle){player->animation.frame.x,player->animation.frame.y,player->animation.frame.width * player->direction.x,player->animation.frame.height},
		(Rectangle){player->position.x,player->position.y,player->animation.frame.width,player->animation.frame.height},
		(Vector2){0,0},
		0,
		WHITE
    );

}

/*
Função para mudar a animação do player de acordo com o estado passado na função
reiniciando os valores do frame atual e do contador de frames do jogo
(Como as animações estão contidas em um unico arquivo grande com varias animações
cada valor do enum States contem a posição em y de cada animação por isso o y do 
frame é atualizado) -> sujeito a alteração
*/
void SetPlayerAnimation(Sprite *player,States state){

    switch (state)
    {   
    case IDLE:
        player->state = state;
        player->animation.frame.y = (int)IDLE * player->animation.texture.height/7;
        player->animation.framesQuantity = 6;
        player->animation.currentFrame = 0;
        player->animation.framesCounter = 0;
        break;
    
    case WALK:
        player->state = state;
        player->animation.frame.y = (int)WALK * player->animation.texture.height/7;
        player->animation.framesQuantity = 8;
        player->animation.currentFrame = 0;
        player->animation.framesCounter = 0;
        break;
    }

}

/*
Função que checa a velocidade e ent chega se o estado de movimento mudou 
para mudar a animação do player entre parado e andando 
*/
void PlayerStatemachine(Sprite *player){

    States newState;

    if(player->velocity.x != 0 || player->velocity.y != 0){
        newState = WALK;
    }else{
        newState = IDLE;
    }

    if(player->state != newState) SetPlayerAnimation(player,newState);

}
