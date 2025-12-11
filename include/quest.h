#ifndef QUEST_H
#define QUEST_H

#include <raylib.h>
#include <player.h>

#define NUMBER_OF_QUESTS 4

typedef enum
{
    NOT_STARTED, 
    IN_PROGRESS,
    COMPLETED,

}QuestStatus;

typedef struct Quest
{
    int id;
    char name[32];
    char description[128];

    int requiredItemId;
    int requiredItemAmount;
    int giftItemId;
    int giftItemAmount;
    
    QuestStatus status;

}Quest;

Quest *GetQuestById(int id);

void StartQuestChoice(void *context);

void AcceptQuestCallback(void *context);

void RejectCallback(void *context);

void GiveQuestItemsCallback(void *context);


#endif
