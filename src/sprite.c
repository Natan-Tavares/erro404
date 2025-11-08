#include <raylib.h>
#include "sprite.h"

/*
    Função para pegar a hitbox do sprite
    Simplesmente retorna um retangulo com a posição do sprite
    *Nota: a width e height estão sujeitas a mudança
    e são asssim devido o tamanho de cada frame da animação
*/
Rectangle GetSpriteHitbox(Sprite self,float width,float height){
    return (Rectangle){
        .x = self.position.x-(width/2),
        .y = self.position.y-(height/2),
        .width = width,
        .height = height,
    };

}

    //Função para atualizar a posição X do sprite em relação a sua velocidade
void applyVelX(Sprite *self){
    self->position.x += self->velocity.x;
}

    //Função para atualizar a posição Y do sprite em relação a sua velocidade
void applyVelY(Sprite *self){
    self->position.y += self->velocity.y;
}

/*
    Função para checar a sobreposição em Y de um sprite e um retangulo 
    É feito o calculo da sobreposição dos dois lados de cada retangulo
    o lado em que a sobreposição for maior significa que não esta sobreposta    
    retornando o outro lado
*/
float CheckCollisionY(Rectangle hitbox,Rectangle collisionRectangle){

    if (CheckCollisionRecs(hitbox, collisionRectangle)) {
            
        float overlapUp  = (hitbox.y + hitbox.height) - collisionRectangle.y;
        float overlapDown = (collisionRectangle.y + collisionRectangle.height) - hitbox.y;

        if (overlapUp < overlapDown) {
            return -overlapUp;
        } else {
            return overlapDown;
        }
    }

    return 0;

}

/*
    Função para checar a sobreposição em X de um sprite e um retangulo
    A função calcula a sobreposição dos dois lados de cada retangulo
    o lado em que a sobreposição for maior significa que não esta sobreposta
    retornando a sobreposição do outro lado
*/
float CheckCollisionX(Rectangle hitbox,Rectangle collisionRectangle){

    if (CheckCollisionRecs(hitbox, collisionRectangle)) {
            
        float overlapLeft  = (hitbox.x + hitbox.width) - collisionRectangle.x;
        float overlapRight = (collisionRectangle.x + collisionRectangle.width) - hitbox.x;

        if (overlapLeft < overlapRight) {
            return -overlapLeft;
        } else {
            return overlapRight;
        }
    }

    return 0;

}
