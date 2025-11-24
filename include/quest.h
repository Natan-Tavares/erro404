#ifndef QUEST_H
#define QUEST_H

#include <raylib.h>
#include <player.h>

#define NUMBER_OF_QUESTS 4

typedef enum
{

    NOT_STARTED = 0, 
    IN_PROGRESS = 1,
    COMPLETED = 2,

}QuestStatus;

typedef struct Quest
{
    int id;
    char name[32];
    char description[128];
    int requiredItemId;
    int numberOfRequiredItem;
    int giftItemId;
    int numberOfGiftItem;
    bool isActive;
    QuestStatus status;

}Quest;

typedef struct GameManager GameManager;

Quest *GetQuestById(int id);

void UpdateQuestChoice(Player *player,GameManager *gameManager);

void DrawQuestChoice(GameManager *gameManager);

#endif
