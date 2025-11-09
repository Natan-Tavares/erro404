#include "game.h"

#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <string.h>

/*
    Função que repoe \ n dentro de uma string por literais \n.
    com duas variaveis apontando para o espaço de memoria da strig
    as variaveis reescrevem a string de acordo com os caracteres
    reescrevendo os \n.
*/
void replaceEscapedNewlines(char *text){
    char *src = text;
    char *dst = text;

    while (*src) {
        if (src[0] == '\\' && src[1] == 'n') {
            *dst++ = '\n';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }

    *dst = '\0';
}

/*
    Função para aproximar um valor inicial de um final usando um fator
*/
float lerp(float start, float end, float t) {
    return start + (end - start) * t;
}

/*
    Função que faz pitagoras entre a distancia x e y de dois vetores para retornar
    a menor distancia entre os dois
*/
float GetDistance(Vector2 a,Vector2 b){

    Vector2 dist = (Vector2){
        .x = (float)abs((a.x - b.x)),
        .y = (float)abs((a.y - b.y))
    };

    return sqrtf((dist.x * dist.x) + (dist.y * dist.y));

}

/*
    Função para checar se foi possivel manipular um arquivo.
*/
bool CheckFile(FILE *file){
    if(file){
        return true;
    }
    perror("Erro No arquivo:");
    return false;
}

/*
	Função para checar se o player acabou de interagir com alguma coisa.
*/
bool CheckJustInteract(GameManager *self){
	if(self->justPressedInteract){
		self->justPressedInteract = false;
		return true;
	}
	return false;
}

/*
    Atualiza os caracteres ativos do dialogo passado de acordo com o delay
    de cada letra.
    *Nota: Faz a animação de letras aparecendo na tela.
*/
void UpdateVisibleChars(char *text,int *visibleChars,float lettersDelay){
    static float timer = 0;
    int textLen = strlen(text);

    timer += GetFrameTime();
    if(timer >= lettersDelay && *visibleChars < textLen){
        (*visibleChars)++;
        timer = 0;

    }
}

void UpdateMenu(GameManager *gameManager){
	
    int titleLength = strlen(gameManager->menu.titleText);

	const char *menuOptions[] = { "START", "EXIT" };

	UpdateVisibleChars(gameManager->menu.titleText,&(gameManager->menu.lettersShown),0.5);

	if (IsKeyPressed(KEY_W)) gameManager->menu.selected--;
    if (IsKeyPressed(KEY_S)) gameManager->menu.selected++;
    if (gameManager->menu.selected < 0) gameManager->menu.selected = gameManager->menu.totalOptions - 1;
    if (gameManager->menu.selected >= gameManager->menu.totalOptions) gameManager->menu.selected = 0;	

    if (IsKeyPressed(KEY_ENTER)) {
        if (gameManager->menu.selected == 0) gameManager->currentScreen = GAME;
        else if (gameManager->menu.selected == 1) gameManager->currentScreen = EXIT;
    }

}

void DrawMenu(GameManager *game){
	int titleFontSize = 60;
	int titleWidth = MeasureText(TextSubtext(game->menu.titleText, 0, game->menu.lettersShown), titleFontSize);
	int titleX = (WINDOW_WIDTH - titleWidth) / 2;   
	int titleY = WINDOW_HEIGHT / 3;

    const char *menuOptions[] = { "START", "EXIT" };

	DrawText(TextSubtext(game->menu.titleText, 0, game->menu.lettersShown), titleX, titleY, titleFontSize, RAYWHITE);

	int optionFontSize = 30;
	int spacing = 50;

	int menuStartY = titleY + 150;
	for (int i = 0; i < game->menu.totalOptions; i++) {
		int textWidth = MeasureText(menuOptions[i], optionFontSize);
		int optionX = (WINDOW_WIDTH - textWidth) / 2;
		int optionY = menuStartY + i * spacing;

		Color color = (i == game->menu.selected) ? WHITE : DARKGRAY;
		DrawText(menuOptions[i], optionX, optionY, optionFontSize, color);

		if (i == game->menu.selected) {
			DrawText(">", optionX - 40, optionY, optionFontSize, WHITE);
		    }
	    }

	const char *hint = "Use W/S para navegar | ENTER para confirmar";
	int hintFontSize = 18;
	int hintWidth = MeasureText(hint, hintFontSize);
	DrawText(hint, (WINDOW_WIDTH - hintWidth) / 2, WINDOW_HEIGHT - 100, hintFontSize, DARKGRAY);

}