#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "Players_data.h"

typedef enum
{
    BATSMAN,
    BOWLER,
    ALL_ROUNDER
} Role;

typedef struct PlayerNode
{
    int playerId;
    char playerName[50];
    char teamName[50];
    Role playerRole;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode* teamNext; 
    struct PlayerNode* roleNext; 
    struct PlayerNode* globalNext; 
} PlayerNode;

typedef struct Team
{
    int teamId;
    char teamName[50];
    int totalPlayers;
    float avgBattingStrikeRate;
    struct PlayerNode* head;
    struct PlayerNode* batsmansHead;
    struct PlayerNode* bowlersHead;
    struct PlayerNode* allRoundersHead;
} Team;

Team teamArray[10];
int isRunning = 1;
PlayerNode* globalBatsman = NULL;
PlayerNode* globalBowler = NULL;
PlayerNode* globalAllRounder = NULL;

void menu ()
{
    printf("\n==============================================================================");
    printf("\nICC ODI Player Performance Analyzer");
    printf("\n==============================================================================");
    printf("\n1. Add Player to Team");
    printf("\n2. Display Players of a Specific Team");
    printf("\n3. Display Teams by Average Batting Strike Rate");
    printf("\n4. Display Top K Players of a Specific Team by Role");
    printf("\n5. Display all Players of Specific Role Across All Teams by Performance Index");
    printf("\n6. Exit");
    printf("\n==============================================================================");
}

Role mapStringtoRole (const char* currRole)
{
    if (strcmp (currRole, "Batsman") == 0)  return BATSMAN;
    else if (strcmp (currRole, "Bowler") == 0) return BOWLER;
    else return ALL_ROUNDER;
}

float calcPerformanceOfPlayer (PlayerNode* currPlayer)
{
    if (currPlayer->playerRole == BATSMAN)
    {
        return (currPlayer->battingAverage * currPlayer->strikeRate) / 100.0f;
    }
    else if (currPlayer->playerRole == BOWLER)
    {
        return ((currPlayer->wickets * 2.0f) + (100.0f - currPlayer->economyRate));
    }

    return (((currPlayer->battingAverage * currPlayer->strikeRate) / 100.0f) + (currPlayer->wickets * 2.0f));
}

int findTeamIndexByName(const char* currTeamName)
{
    for (int index = 0; index < teamCount; index++)
    {
        if (strcmp (currTeamName, teamArray[index].teamName) == 0)
        {
            return index;
        }
    }
    return -1;
}

int binarySearchByTeamId (int searchId)
{
    int low = 0, high = teamCount - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (teamArray[mid].teamId == searchId)
        {
            return mid;
        }
        else if (teamArray[mid].teamId > searchId)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}

void reCalcTeamAverage (int teamIndex)
{
    float avgSum = 0;
    int playerCount = 0;
    PlayerNode* temp = teamArray[teamIndex].head;
    while (temp)
    {
        if (temp->playerRole == BATSMAN || temp->playerRole == ALL_ROUNDER)
        {
            avgSum += temp->strikeRate;
            playerCount++;
        }
        temp = temp->teamNext;
    }
    teamArray[teamIndex].avgBattingStrikeRate = ((playerCount == 0) ? 0 : avgSum / playerCount); 
}

void insertRoleSortedOrder (PlayerNode** head, PlayerNode* newNode)
{
    if (*head == NULL || (*head)->performanceIndex < newNode->performanceIndex)
    {
        newNode->roleNext = *head;
        *head = newNode;
        return;
    }
    PlayerNode* temp = *head;
    while (temp->roleNext != NULL && temp->roleNext->performanceIndex >= newNode->performanceIndex)
    {
        temp = temp->roleNext;
    }
    newNode->roleNext = temp->roleNext;
    temp->roleNext = newNode;
}

void insertGlobalSortedOrder (PlayerNode** head, PlayerNode* newNode)
{
    if (*head == NULL || (*head)->performanceIndex < newNode->performanceIndex)
    {
        newNode->globalNext = *head;
        *head = newNode;
        return;
    }
    PlayerNode* temp = *head;
    while (temp->globalNext != NULL && temp->globalNext->performanceIndex >= newNode->performanceIndex)
    {
        temp = temp->globalNext;
    }
    newNode->globalNext = temp->globalNext;
    temp->globalNext = newNode;
}

void insertPlayerIntoGlobal (Role currRole, PlayerNode* newPlayer)
{
    if (currRole == BATSMAN)
    {
        insertGlobalSortedOrder (&globalBatsman, newPlayer);
    }
    else if (currRole == BOWLER)
    {
        insertGlobalSortedOrder (&globalBowler, newPlayer);
    }
    else if (currRole == ALL_ROUNDER)
    {
        insertGlobalSortedOrder (&globalAllRounder, newPlayer);
    }
}

void insertPlayerIntoTeam (int teamIndex, PlayerNode* currPlayer)
{
    currPlayer->teamNext = teamArray[teamIndex].head;
    teamArray[teamIndex].head = currPlayer;
    teamArray[teamIndex].totalPlayers++;
    if (currPlayer->playerRole == BATSMAN)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].batsmansHead, currPlayer);
    }
    else if (currPlayer->playerRole == BOWLER)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].bowlersHead, currPlayer);
    }
    else if (currPlayer->playerRole == ALL_ROUNDER)
    {
        insertRoleSortedOrder(&teamArray[teamIndex].allRoundersHead, currPlayer);
    }
    reCalcTeamAverage (teamIndex);
}

void initializeSystem ()
{
    for (int index = 0; index < teamCount; index++)
    {
        teamArray[index].teamId = index + 1;
        teamArray[index].batsmansHead = NULL;
        teamArray[index].head = NULL;
        teamArray[index].bowlersHead = NULL;
        teamArray[index].allRoundersHead = NULL;
        strcpy(teamArray[index].teamName, teams[index]);
        teamArray[index].totalPlayers = 0;
        teamArray[index].avgBattingStrikeRate = 0;
    }

    for (int index = 0; index < playerCount; index++)
    {
        int teamIndex = findTeamIndexByName(players[index].team);
        PlayerNode* newPlayer = malloc (sizeof(PlayerNode));
        newPlayer->playerId = players[index].id;
        strcpy(newPlayer->playerName, players[index].name);
        strcpy(newPlayer->teamName, players[index].team);
        newPlayer->playerRole = mapStringtoRole (players[index].role);
        newPlayer->battingAverage = players[index].battingAverage;
        newPlayer->economyRate = players[index].economyRate;
        newPlayer->strikeRate = players[index].strikeRate;
        newPlayer->totalRuns = players[index].totalRuns;
        newPlayer->wickets = players[index].wickets;
        newPlayer->teamNext = NULL;
        newPlayer->roleNext = NULL;
        newPlayer->globalNext = NULL;
        newPlayer->performanceIndex = calcPerformanceOfPlayer (newPlayer);
        insertPlayerIntoTeam (teamIndex, newPlayer);
        insertPlayerIntoGlobal (newPlayer->playerRole, newPlayer);
    }
}

char* getStringFromRole (Role currRole)
{
    if (currRole == BATSMAN) return "Batsman";
    else if (currRole == BOWLER) return "Bowler";
    return "All-rounder";
}

void displayTeamPlayersById (int teamId)
{
    int teamIndex = binarySearchByTeamId(teamId);
    if (teamIndex == -1)
    {
        printf("\nTeam Not Found!");
        return;
    }
    PlayerNode* currPlayer = teamArray[teamIndex].head;
    printf("\nPlayers of Team %s:", teamArray[teamIndex].teamName);
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", currPlayer->playerId, currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), currPlayer->totalRuns,
            currPlayer->battingAverage, currPlayer->strikeRate,
            currPlayer->wickets, currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->teamNext;
    }
    printf("\n======================================================================================");
    printf("\nTotal Players: %d", teamArray[teamIndex].totalPlayers);
    printf("\nAverage Batting Strike Rate: %.2f", teamArray[teamIndex].avgBattingStrikeRate);
}

void displayTeamsByAvgStrikeRate ()
{
    Team sortedTeamsArray[teamCount];
    for (int index = 0; index < teamCount; index++)
    {
        sortedTeamsArray[index] = teamArray[index];
    }
    for (int index1 = 0; index1 < teamCount; index1++)
    {
        for(int index2 = 0; index2 < teamCount - index1 - 1; index2++)
        {
            if (sortedTeamsArray[index2].avgBattingStrikeRate < sortedTeamsArray[index2 + 1].avgBattingStrikeRate)
            {
                Team temp = sortedTeamsArray[index2];
                sortedTeamsArray[index2] = sortedTeamsArray[index2 + 1];
                sortedTeamsArray[index2 + 1] = temp;
            }
        }
    }
    printf("\nTeams Sorted by Average Batting Strike Rate");
    printf("\n=============================================================");
    printf("\nID    Team Name            Avg Bat SR   Total Players");
    printf("\n=============================================================");
    for (int index = 0; index < teamCount; index++)
    {
        printf("\n%-5d %-20s   %-15.1f%-5d", sortedTeamsArray[index].teamId, sortedTeamsArray[index].teamName,
            sortedTeamsArray[index].avgBattingStrikeRate, sortedTeamsArray[index].totalPlayers);
    }
    printf("\n=============================================================");
}

char* getStringForRole (int currRole)
{
    if (currRole == 1) return "Batsman";
    else if (currRole == 2) return "Bowler";
    return "All-rounder";
}

void addNewPlayerToTeam ()
{
    int targetTeamId;
    printf("\nEnter Team ID to add player: ");
    scanf("%d", &targetTeamId);
    int teamIndex = binarySearchByTeamId (targetTeamId), currRole;
    if (teamIndex > 9 || teamIndex < 0)
    {
        printf("\nNo Such Team Found!");
        return;
    }
    PlayerNode* newPlayer = malloc (sizeof(PlayerNode));
    strcpy(newPlayer->teamName, teamArray[teamIndex].teamName);
    printf("\nEnter PLayer Details: ");
    printf("\nPlayer ID: ");
    scanf("%d", &(newPlayer->playerId));
    getchar();
    printf("\nName: ");
    fgets(newPlayer->playerName, 50, stdin);
    (newPlayer->playerName)[strcspn(newPlayer->playerName, "\n")] = '\0';
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    newPlayer->playerRole = mapStringtoRole (getStringForRole(currRole));
    printf("\nTotal Runs: ");
    scanf("%d", &(newPlayer->totalRuns));
    printf("\nBatting Average: ");
    scanf("%f", &(newPlayer->battingAverage));
    printf("\nStrike Rate: ");
    scanf("%f", &(newPlayer->strikeRate)); 
    printf("\nWickets: ");
    scanf("%d", &(newPlayer->wickets));
    printf("\nEconomy Rate: ");
    scanf("%f", &(newPlayer->economyRate));
    newPlayer->roleNext = NULL;
    newPlayer->teamNext = NULL;
    newPlayer->globalNext = NULL;
    newPlayer->performanceIndex = calcPerformanceOfPlayer (newPlayer);

    insertPlayerIntoTeam (teamIndex, newPlayer);
    insertPlayerIntoGlobal (newPlayer->playerRole, newPlayer);
    printf("\nPlayer Added Successfully to Team %s!", teamArray[teamIndex].teamName);
}

void displayTopKPlayers ()
{
    int currTeamId, currRole, topK;
    printf("\nEnter Team ID: ");
    scanf("%d", &currTeamId);
    int teamIndex = binarySearchByTeamId (currTeamId);
    if (teamIndex > 9 || teamIndex < 0)
    {
        printf("\nNo Such Team Found!");
        return;
    }
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    if (currRole > 3 || currRole < 1)
    {
        printf("\nEnter Valid Role!");
        return;
    }
    printf("\nEnter number of players: ");
    scanf("%d", &topK);
    PlayerNode* currPlayer;
    if (currRole == 1) currPlayer = teamArray[teamIndex].batsmansHead;
    else if (currRole == 2) currPlayer = teamArray[teamIndex].bowlersHead;
    else if (currRole == 3) currPlayer = teamArray[teamIndex].allRoundersHead;
    printf("\nTop %d %s of Team %s:", topK, getStringForRole(currRole), teamArray[teamIndex].teamName);
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer != NULL && topK--)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", currPlayer->playerId, currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), currPlayer->totalRuns,
            currPlayer->battingAverage, currPlayer->strikeRate,
            currPlayer->wickets, currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->roleNext;
    }
}

void displayAllPlayersByRole ()
{
    int currRole;
    printf("\nRole (1-Batsman, 2-Bowler, 3-All-Rounder): ");
    scanf("%d", &currRole);
    if (currRole > 3 || currRole < 1)
    {
        printf("\nEnter Valid Role!");
        return;
    }
    PlayerNode* currPlayer;
    if (currRole == 1) currPlayer = globalBatsman;
    else if (currRole == 2) currPlayer = globalBowler;
    else if (currRole == 3) currPlayer = globalAllRounder;
    printf("\n%s of All Teams: ", getStringForRole (currRole));
    printf("\n======================================================================================");
    printf("\nID    Name                    Role         Runs  Avg    SR   Wkts   ER   Perf. Index");
    printf("\n======================================================================================");
    while (currPlayer != NULL)
    {
        printf("\n%-5d %-23s %-12s %-5d %-7.1f %-5.1f %-5d %-5.1f %-5.1f", currPlayer->playerId, currPlayer->playerName,
            getStringFromRole(currPlayer->playerRole), currPlayer->totalRuns,
            currPlayer->battingAverage, currPlayer->strikeRate,
            currPlayer->wickets, currPlayer->economyRate,
            currPlayer->performanceIndex);
            currPlayer = currPlayer->globalNext;
    }
}

void releaseMemory ()
{
    PlayerNode* temp = NULL;
    PlayerNode* curr = globalBowler;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
    curr = globalBatsman;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
    curr = globalAllRounder;
    do
    {
        temp = curr->globalNext;
        free(curr);
        curr = temp;
    } while (temp);
}

void main ()
{
    initializeSystem();
    while (isRunning)
    {
        int choice;
        menu ();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
        case 1:
            addNewPlayerToTeam ();
            break;
        case 2:
            printf("\nEnter Team ID: ");
            int searchTeamId;
            scanf("%d", &searchTeamId);
            displayTeamPlayersById (searchTeamId);
            break;
        case 3:
            displayTeamsByAvgStrikeRate ();
            break;
        case 4:
            displayTopKPlayers ();
            break;
        case 5:
            displayAllPlayersByRole ();
            break;
        case 6:
            releaseMemory ();
            printf("Releasing Memory........Exiting Program!");
            isRunning = 0;
            break;
        }
    }
}