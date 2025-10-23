#include <raylib.h>
#include <sprite.h>
#include <player.h>
#include <animation.h>

int main(){

	const int screenWidth = 800;
	const int screenHeight = 450;

	//Iniciando a janela
	Vector2 windowCenter = {(float)screenWidth/2,(float)screenHeight/2};
	InitWindow(screenWidth,screenHeight,"main");

	//criando o player e definido suas animações
	Sprite player;

	player.animation = (animation){
		LoadTexture("resources/soldier.png"),
		{},
		8,
		0,
	};

	//criando retangulos para teste
	Rectangle frameRectangle = {0,0,(float)player.animation.texture.width/9,(float)player.animation.texture.height/7};
	Rectangle collisionTeste = {50,50,32,32};
	
	//Definindo defaults
	player.speed = 2;
	player.xSpriteDirection = 1;
	player.position.x = windowCenter.x;
	player.position.y = windowCenter.y;

	player.animation.frame = frameRectangle;

	//Definindo o fps do jogo
	SetTargetFPS(60);

	while(!WindowShouldClose()){

		PlayerStatemachine(&player);

		animate(&player.animation);
		
		MovePlayer(&player);

		applyVelX(&player);
		checkCollisionX(&player,collisionTeste);

		applyVelY(&player);
		checkCollisionY(&player,collisionTeste);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			DrawFPS(10,10);

			DrawTexturePro(
				player.animation.texture,
				(Rectangle){player.animation.frame.x,player.animation.frame.y,player.animation.frame.width * GetPlayerDirection(&player),player.animation.frame.height},
				(Rectangle){player.position.x,player.position.y,100,100},
				(Vector2){0,0},
				0,
				WHITE
			);
			DrawRectangleRec(collisionTeste,GREEN);

		EndDrawing();

	}

	UnloadTexture(player.animation.texture);

	return 0;
}
