#include "npc.h"

#include <tilemap.h>
#include <game.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <player.h>

/*
    conta o numero de npcs em um arquivo, passado o arquivo para leitura
    le cada linha e se ela começar com um # soma +1 a quantidade de npcs.
    Nota: cada # é um npc é possivel se escrever coisas dps do # mas mantenha
    um # por npc.
*/
int CountNpcs(FILE *file){

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line,sizeof(line),file))
    {

        if(line[0] == '#') count++;
    }
    
    rewind(file);
    return count;

}

/*
    Função que preenche os valores de um npc.
    recebendo um ponteiro para um npc e uma string,
    analisa a string procurando pelos dados especificos do npc
    Então pega os dados e preenche os campos do npc.
*/
void fillNpcValues(Npc *npc,char *line){

    if(!strncmp(line,"id:",3)){
        npc->dialogueCount = 0;
        npc->id = atoi(line+3);
        
    }else if(!strncmp(line,"position:",9)){
        sscanf(line+9,"%f,%f",&(npc->position.x),&(npc->position.y));

    }else if(!strncmp(line,"dialogue:",9)){
        const char *dialogueText = line + 9;

        int currentDialogue = npc->dialogueCount;
        strncpy(npc->dialogues[currentDialogue].text,dialogueText, MAX_DIALOGUE_LENGTH - 1);

        npc->dialogues[currentDialogue].text[MAX_DIALOGUE_LENGTH - 1] = '\0';

        replaceEscapedNewlines(npc->dialogues[currentDialogue].text);

        npc->dialogueCount++; 
    }

}

/*
    Função para ler um arquivo com as informações dos npcs
    le linha por linha do arquivo passando cada linha para a função
    de preencher os valos do npc, checando se tem um # antes de cada 
    declaração de npc,aumenta o indice da lista  
*/
void ReadNpcs(Npc *npcList,FILE *file){

    char line[MAX_LINE_LENGTH];
    int index = -1;

    while(fgets(line,sizeof(line),file))
    {
        if(strlen(line) == 0) continue; //checa se a linha esta vazia

        if(line[0] == '#'){ 
            index++; //se tiver um # aumenta o indice da lista de npcs
          continue;  
        } 

        line[strcspn(line, "\n")] = '\0'; //retira o ultimo \n da string da linha

        fillNpcValues(&npcList[index],line); //preenche os valores do npc de acordo com a linha passada

    }

}

/*
    Função para carregar a lista de npcs no arquivo de npcs
    Recebendo o nome do arquivo que contem as caracteristicas do npcs
    abre o arquivo, checa se ele é manipulavel, conta o numero de npcs no arquivo
    malloca o numero de npcs para uma lista de npcs, preenche essa lista de npcs
    com a função ReadNpcs e então retorna a lista.
    *Nota: Lembre-se de dar free na lista de npcs no final do programa
*/
Npc *LoadNpcs(const char* filename,int *numberOfNpcs){

    char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename,"r");

    if(!CheckFile(file)) return NULL;

    *numberOfNpcs = CountNpcs(file); //conta o numero de npcs

    Npc *npcs = malloc(sizeof(Npc) * (*numberOfNpcs));

    ReadNpcs(npcs,file); //le o arquivo preenchendo os npcs  na lista

    fclose(file);
    return npcs;
}


/*
    Função para Desenhar os npcs no mapa.
    Recebendo a lista de npcs que deseja desenhar e o numero de npcs,
    checa a variavel isPlayerNearby para saber se o player esta proximo
    e muda a sua cor de acordo com a proximidade do player;s
*/
void DrawNpcs(Npc *npcList,int numberOfNpcs){

    for(int i = 0;i < numberOfNpcs;i++){
        Color color = npcList[i].isPlayerNearby ? YELLOW : GREEN;

        DrawRectangle(
            npcList[i].position.x,
            npcList[i].position.y,
            TILE_SIZE,
            TILE_SIZE,
            color
        );

    }
}

/*
    Função para atualizar o valor do isPlayerNearby dos npcs
    Recebendo o npc para checar,o sprite do plyer e uma distancia de detecção
    Calcula a distancia do npc para o player e checa se ele é menor que o raio
    de detecção
*/
void UpdateNpcProximity(Npc *npc,Object object, float detectionRange) {
    float dist = GetDistance(npc->position, object.position);
    npc->isPlayerNearby = (dist <= detectionRange);

}


/*
    Atualiza a poximidade de todos os npcs
    Recebendo a lista de npcs, passa npc por npc para a função UpdateNpcProximity
*/
void CheckAllNpcProximities(Npc *npcList,Player player,GameManager gameManager){
    for (int i = 0; i < gameManager.numberOfNpcs; i++) {
        UpdateNpcProximity(&npcList[i], player.object, 60);

    }
}

/*
    Função para atualizar os valores do npc ativo no Gerenciador do jogo
    Recebendo o npc, o index do dialogo ativo, e o gerenciador do jogo,
    atualiza o gerenciador para dizer qual o npc ativo, inicializando as variaveis do seu dialogo.
*/
void StartNpcDialogue(Npc *npc,int dialogueIndex,GameManager *gameManager){
    if(!npc) return;

    gameManager->activeNpc = npc;
    gameManager->activeDialogueindex = dialogueIndex;
    gameManager->justPressedInteract = true;

    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);
    activeDialogue->visibleChars = 0;
    activeDialogue->activate = true;
    
}


/*
    Função para checar se o npc tem dialogos.
    Recebendo o npc e o gerenciador do jogo, checa se o npc passado tem dialogos
    e se tiver chama a função StartNpcDialogue para iniciar o primeiro dialogo. 
*/
void TalkToNpc(Npc *npc,GameManager *game){
    if(!npc) return;

    if (npc->dialogueCount > 0){
        StartNpcDialogue(npc,0,game);
    }
    
}

/*
    Termina o dialogo com o npc definindo que o valor do npc ativo no gerenciado jogo
    é NULL.
*/
void StopNpcDialogue(GameManager *gameManager){
    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);

    activeDialogue->activate = false;
    activeDialogue->visibleChars = 0;

    gameManager->activeDialogueindex = 0;
    gameManager->activeNpc = NULL;

}

/*
    Função para checar o dialogo ativo.
    Recebendo o Gerenciador do jogo, checa se o dialogo do npc ativo esta realmente ativo, se estiver
    retorna true, se não estiver ativo, define que o npc ativo é NULL e retorna false.
*/
bool CheckActiveDialogue(GameManager *gameManager){

    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);

    if(activeDialogue->activate){
        return true; //se o dialogo ativo estiver realmente ativo, retorna true;
    }

    gameManager->activeNpc = NULL; //se o dialogo ativo não estiver realmente ativo, retina o npc de npc ativo e retorna falso
    return false;
}

/*
    Função para passar para o proximo dialogo.
    Recebendo o gerenciador do jogo e o Dialogo ativo, Checa se o numero de letras
    a mostra do dialogo ativo é maior ou igual ao tamanho do texto e se a tecla de 
    interação foi pressionada, então checa se esta no ultimo dialogo para fechar o 
    dialogo ou se para o proximo dialogo.
*/
void GoToNextDialogue(GameManager *gameManager,Dialogue *activeDialogue){

    int textLen = strlen(activeDialogue->text);

    if(activeDialogue->visibleChars >= textLen && IsKeyPressed(KEY_E)){
        if(gameManager->activeDialogueindex + 1 < gameManager->activeNpc->dialogueCount){
            activeDialogue->activate = false;
            StartNpcDialogue(gameManager->activeNpc,gameManager->activeDialogueindex +1,gameManager); //atualiza os valores do dialogo do npc
            return;
        
        } else {
            StopNpcDialogue(gameManager);
            return;

        }
    
    }   
}

/*
    Função para pular um dialogo.
    Checa se o numero de caracteres amostra de um dialogo é menor
    que o tamanho do dialogo, se for e o player apertar o botão de interação
    ele iguala o numero de caracteres visiveis ao tamanho do texto
*/
void JumpDialogue(Dialogue *activeDialogue){
    int textLen = strlen(activeDialogue->text);

    if(activeDialogue->visibleChars < textLen && IsKeyPressed(KEY_E)){
        activeDialogue->visibleChars = textLen;
    }    
}

/*
    Função para atualizar o dialogo ativo.
    Função para checar se existe um dialogo ativo e se existir
    chamar as outras funções.
*/
void UpdateActiveDialogue(GameManager *gameManager){
    if(!gameManager->activeNpc) return; //Caso não tenha nenhum npc ativo ele não atualiza

    if(CheckJustInteract(gameManager)) return; //Se ele tiver acabado de interagir não atualiza o dialogo

    if(!CheckActiveDialogue(gameManager)) return; //Se o dialogo ativo não estiver realmente ativo retorna

    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);
    
    UpdateVisibleChars(activeDialogue->text,&(activeDialogue->visibleChars),0.05);

    GoToNextDialogue(gameManager,activeDialogue);

    JumpDialogue(activeDialogue);
}

/*
    Fução para desenhar o dialogo ativo.
    Recebendo o gerenciado do jogo, Checa se existe um npc ativo e se existir,
    desenha na tela, tanto a caixa preta do dialogo, quanto o dialogo em si.
*/
void DrawActiveDialogue(GameManager *gameManager){    
    if(!gameManager->activeNpc) return;

    if(!CheckActiveDialogue(gameManager)) return; //Caso o dialogo não esteja ativo a função não faz nada

    Dialogue *activeDialogue = &(gameManager->activeNpc->dialogues[gameManager->activeDialogueindex]);

    DrawRectangle(50,400,700,150,Fade(BLACK,0.8));

    char buffer[MAX_DIALOGUE_LENGTH];
    strncpy(buffer,activeDialogue->text,activeDialogue->visibleChars);
    buffer[activeDialogue->visibleChars] = '\0';

    DrawText(buffer,70, 420, 20, WHITE);

}