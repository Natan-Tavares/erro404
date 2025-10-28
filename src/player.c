#include "player.h"
#include <raylib.h>
#include <stdio.h>
#include <sprite.h>
#include <tilemap.h>

/*
    Função para atualizar a velocidade do player de acordo com o input,
    multiplicando a direção do input pela speed do sprite
*/
void MovePlayer(Sprite *self){

    float xDirection = (int)IsKeyDown(KEY_D) - (int)IsKeyDown(KEY_A);
    float yDirection = (int)IsKeyDown(KEY_S) - (int)IsKeyDown(KEY_W);

    self->velocity.x = xDirection * self->speed;
    self->velocity.y = yDirection * self->speed;

}

/*
    Função para mudar o valor da direção em que o player esta virado de acordo com a velocidade
    (lembre-se de iniciar a direção com um valor default)
*/
void GetPlayerDirection(Sprite *self){

    if(self->velocity.x > 0){
        self->direction.x = 1;
    }else if(self->velocity.x < 0){
        self->direction.x = -1;
    }

}

/*
    Função que para diminuir o tamanho da função de DrawTexturePro 
    e virar o player de acordo com a direção em que esta se movendo
    (multiplicando o valor do largura do frame que ele desenha pela direção em que esta virado) 
*/
void DrawPlayer(Sprite *self,Texture2D textura){
    GetPlayerDirection(self);

    Rectangle animationFrame = GetAnimationFrame(&(self->animation),9); 
    animationFrame.width *= self->direction.x;

    Rectangle dest = GetAnimationFrame(&(self->animation),9);
    dest.x = self->position.x;
    dest.y = self->position.y;

    DrawTexturePro(
	    textura,
		animationFrame,
		dest,
		(Vector2){50,50},
		0,
		WHITE
    );

}

/*
    Função para mudar a animação do player de acordo com o estado passado na função
    reiniciando os valores do frame atual e do contador ate o proximo frame da animação
*/
void SetPlayerAnimation(animation *self,state state){
    switch (state){   
        case IDLE:
            self->state = state;
            self->last = 6;
            break;
        
        case WALK:
            self->state = state;;
            self->last = 8;
            break;
    }
    self->current = 0;
    self->durationLeft = 0.1;
}

/*
    Função que checa a velocidade e então checa se o estado de movimento mudou
    caso tenha mudado, muda a animação do player entre parado e andando.
*/
void PlayerStatemachine(Sprite *self){

    state newState;

    if(self->velocity.x != 0 || self->velocity.y != 0){
        newState = WALK;
    }else{
        newState = IDLE;
    }

    if(self->animation.state != newState) SetPlayerAnimation(&(self->animation),newState);

    }

Vector2 GetPlayerTile(Sprite *self){

    return (Vector2){

    .x = (int)self->position.x/TILE_SIZE,
    .y = (int)self->position.y/TILE_SIZE

    };

}