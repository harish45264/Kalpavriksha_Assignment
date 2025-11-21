#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Node
{
    int key;
    char value[100];
    bool occupied;
    struct Node* next;
    struct Node* previous;
} Node;

typedef struct Queue
{
    struct Node* front;
    struct Node* rear;
} Queue;

Node** cacheTable = NULL;
Queue* queue = NULL;
static int capacity = 0;
static int currSize = 0;
static int isRunning = 1;

void initializeQueue ()
{
    queue = malloc(sizeof(Queue));
    if (queue == NULL)
    {
        printf("\nError While Allocating Memory!");
        return;
    }
    queue->front = NULL;
    queue->rear = NULL;
}

bool isEmpty ()
{
    return (queue->front == NULL);
}

void createCacheTable (int newCapacity)
{
    capacity = newCapacity;
    cacheTable = calloc (capacity, sizeof(Node*));
    if (cacheTable == NULL)
    {
        printf("Error while allocating memory!\n");
        return;
    }
    printf("Cache Table created!\n");
}

int getIndex (int key)
{
    return (key % capacity);
}

void insertCache (Node* newNode)
{
    newNode->next = NULL;
    newNode->previous = queue->rear;

    if (queue->rear)
    {
        queue->rear->next = newNode;
    }
    queue->rear = newNode;
    if (queue->front == NULL)
    {
        queue->front = newNode;
        newNode->previous = NULL;
    }
}

void removeCache (Node* currNode)
{
    Node* node = currNode;
    if (node == NULL) return; 
    if (queue->front == node)
        queue->front = node->next;
        
    if (queue->rear == node)
        queue->rear = node->previous;
        
    if (node->previous)
        node->previous->next = node->next;
        
    if (node->next)
        node->next->previous = node->previous;
        
    if (queue->front)
        queue->front->previous = NULL;
        
    if (queue->rear)
        queue->rear->next = NULL;
        
    node->next = node->previous = NULL;
}

void get (int key)
{
    int hashIndex = getIndex (key);
    Node* currNode = cacheTable[hashIndex];
    if (currNode != NULL && currNode->occupied && currNode->key == key)
    {
        removeCache (currNode);
        insertCache (currNode);
        printf("%s\n", currNode->value);
        return;
    }
    else
    {
        printf("NULL\n");
        return;
    }
}

void put (int currKey, char* currValue)
{
    int hashIndex = getIndex (currKey);
    if (cacheTable[hashIndex] != NULL && cacheTable[hashIndex]->occupied)
    {
        if (cacheTable[hashIndex]->key == currKey)
        {
            Node* currNode = cacheTable[hashIndex];
            strcpy(currNode->value, currValue);
            removeCache(currNode);
            insertCache(currNode);
            return;
        }
    }
    while (currSize >= capacity)
    {
        Node* old = queue->front;
        int oldIndex = getIndex(old->key);
        if (cacheTable[oldIndex] != NULL && cacheTable[oldIndex]->key == old->key)
            cacheTable[oldIndex] = NULL;
        removeCache(old);
        free(old);
        currSize--;
    }
    Node* newNode = malloc(sizeof(Node));
    newNode->key = currKey;
    newNode->occupied = true;
    newNode->next = NULL;
    newNode->previous = NULL;
    strcpy(newNode->value, currValue);
    cacheTable[hashIndex] = newNode;
    insertCache (newNode);
    currSize++;
}

void main ()
{
    initializeQueue ();
    char cmd[15];
    while (isRunning)
    {
        scanf("%s", cmd);
        if (strcmp(cmd, "createCache") == 0)
        {
            int newCapacity;
            scanf("%d", &newCapacity);
            createCacheTable (newCapacity);
        }
        else if (strcmp(cmd, "put") == 0)
        {
            int newKey;
            char value[100];
            scanf("%d", &newKey);
            scanf("%s", value);
            put(newKey, value);
        }
        else if (strcmp(cmd, "get") == 0)
        {
            int currKey;
            scanf("%d", &currKey);
            get(currKey);
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            isRunning = 0;
            break;
        }
        else
        {
            printf("Enter Valid Command!\n");
        }
    }
}