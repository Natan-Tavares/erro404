
#include <raylib.h>
#include "animation.h"

void animate(animation *animation){

    	animation->framesCounter++;

		if(animation->framesCounter >= (60/animation->frameSpeed)){
			animation->framesCounter = 0;
			animation->currentFrame++;

			if (animation->currentFrame > (animation->framesQuantity-1)) animation->currentFrame = 0;

			animation->frame.x = (float)animation->currentFrame*(float)animation->texture.width/9;
		}
}

void changeAnimation(animation *animation,char *fileName,int framesQuantity){

    UnloadTexture(animation->texture);

    animation->texture = LoadTexture(fileName);

    animation->framesQuantity = framesQuantity;
    animation->currentFrame = 0;
    animation->framesCounter = 0;

}
