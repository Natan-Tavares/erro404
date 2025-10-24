
#include <raylib.h>
#include "animation.h"

void animate(animation *animation){

    	animation->framesCounter++;

		if(animation->framesCounter >= (60/animation->frameSpeed)){
			animation->framesCounter = 0;
			animation->currentFrame++;

			if (animation->currentFrame > (animation->framesQuantity-1)) animation->currentFrame = 0;

			animation->frame.x = (float)animation->currentFrame*(float)animation->texture.width/animation->textureQuantityFrames.x;
		}
}

void changeAnimation(animation *animation,Texture novaTextura,int framesQuantity){

    animation->texture = novaTextura;

    animation->framesQuantity = framesQuantity;
    animation->currentFrame = 0;
    animation->framesCounter = 0;

}
