#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 1024
#define NAME_LEN 50
#define MAX_BLOCK_PER_FILE 20

typedef struct freeBlock
{
    int index;
    struct freeBlock* next;
    struct freeBlock* previous;
}freeBlock;

typedef struct fileNode
{
    char fileName[NAME_LEN];
    int size;
    bool isDirectory;
    struct fileNode* next;
    struct fileNode* previous;
    struct fileNode* parent;
    struct fileNode* childHead;
    int blockPointers[MAX_BLOCK_PER_FILE];
}fileNode;

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];
freeBlock* freeBlockHead = NULL;
freeBlock* freeBlockTail = NULL;
fileNode* rootDir = NULL;
fileNode* currDir = NULL;
static int gIsRunning = 1;

void initializeFreeBlocks ()
{
    for (int index = 0; index < NUM_BLOCKS; index++)
    {
        freeBlock* block = malloc (sizeof(freeBlock));
        block->index = index;
        block->next = NULL;
        block->previous = freeBlockTail;

        if (freeBlockTail != NULL)
        {
            freeBlockTail->next = block;
        }
        else
        {
            freeBlockHead = block;
        }
        freeBlockTail = block;
    }
}

void initializeRootDirectory ()
{
    rootDir = malloc (sizeof(fileNode));
    strcpy (rootDir->fileName, "/");
    rootDir->isDirectory = true;
    rootDir->next = NULL;
    rootDir->parent = NULL;
    rootDir->childHead = NULL;
    rootDir->previous = NULL;
    rootDir->size = 0;
    currDir = rootDir;
}

int allocateFreeBlock ()
{
    if (freeBlockHead == NULL)
    {
        return -1;
    }
    int index = freeBlockHead->index;
    freeBlock* temp = freeBlockHead;
    freeBlockHead = freeBlockHead->next;
    if (freeBlockHead != NULL)
    {
        freeBlockHead->previous = NULL;
    }
    else
    {
        freeBlockTail = NULL;
    }
    free(temp);
    return index;
}

void freeAllocatedBlock (int index)
{
    freeBlock* newNode = malloc(sizeof(freeBlock));
    newNode->index = index;
    newNode->next = NULL;
    newNode->previous = freeBlockTail;
    if (freeBlockTail != NULL)
    {
        freeBlockTail->next = newNode;
    }
    else
    {
        freeBlockHead = newNode;
    }
    freeBlockTail = newNode;
}

fileNode* findNode (fileNode* directory, char* fileName)
{
    if (directory->childHead == NULL)
    {
        return NULL;
    }
    fileNode* temp = directory->childHead;
    do
    {
        if (strcmp (temp->fileName, fileName) == 0)
        {
            return temp;
        }
        temp = temp->next;
    } while (temp != directory->childHead);
    return NULL;
}

void insertChild (fileNode* parent, fileNode* child)
{
    if (parent->childHead == NULL)
    {
        parent->childHead = child;
        child->previous = child;
        child->next = child;
        return;
    }
    fileNode* head = parent->childHead;  
    fileNode* tail = parent->childHead->previous;
    tail->next = child;
    head->previous = child;
    child->previous = tail;
    child->next = head;
    return;
}

void mkdirCommand (char* fileName) 
{
    if (findNode(currDir, fileName) != NULL)
    {
        printf("\nDirectory Already exists!");
        return;
    }
    fileNode* newChild = malloc(sizeof(fileNode));
    strcpy(newChild->fileName, fileName);
    newChild->isDirectory = true;
    newChild->next = NULL;
    newChild->previous = NULL;
    newChild->parent = currDir;
    newChild->size = 0;
    newChild->childHead = NULL;
    insertChild (currDir, newChild);
    printf("\nDirectory %s created successfully.", fileName);
}

void createCommand (char* fileName)
{
    if (findNode(currDir, fileName) != NULL)
    {
        printf("\nFile already exists!");
        return;
    }
    fileNode* newChild = malloc(sizeof(fileNode));
    strcpy(newChild->fileName, fileName);
    newChild->isDirectory = false;
    newChild->next = NULL;
    newChild->previous = NULL;
    newChild->parent = currDir;
    newChild->size = 0;
    newChild->childHead = NULL;
    for (int index = 0; index < MAX_BLOCK_PER_FILE; index++)
    {
        newChild->blockPointers[index] = -1;
    }
    insertChild (currDir, newChild);
    printf("\nFile %s created Successfully.", fileName);
}

void writeCommand (char* fileName, char* data)
{
    fileNode* currFile = findNode (currDir, fileName);
    if (currFile == NULL || currFile->isDirectory)
    {
        printf("\nFile Not Found.");
        return;
    }
    int dataLength = strlen (data);
    int blocksRequired = (dataLength + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (blocksRequired > MAX_BLOCK_PER_FILE)
    {
        printf("\nFile too large!");
        return;
    }
    for (int index = 0; index < blocksRequired; index++)
    {
        int blockIndex = allocateFreeBlock();
        if (blockIndex == -1)
        {
            printf("\nDisk full!");
            return;
        }
        currFile->blockPointers[index] = blockIndex;
        strncpy (virtualDisk[blockIndex], (data + (index * BLOCK_SIZE)), BLOCK_SIZE);
        virtualDisk[blockIndex][BLOCK_SIZE - 1] = '\0';
    }
    printf("\nData written successfully (size=%d bytes)!", dataLength);
    return;
}

void readCommand (char* fileName)
{
    fileNode* currFile = findNode (currDir, fileName);
    if (currFile == NULL || currFile->isDirectory)
    {
        printf("\nFile not exists!");
        return;
    }
    printf("\n");
    for (int index = 0; index < MAX_BLOCK_PER_FILE; index++)
    {
        int blockIndex = currFile->blockPointers[index];
        if (blockIndex == -1)
        {
            break;
        }
        printf("%s", virtualDisk[blockIndex]);
    }
    return;
}

void deleteCommand (char* fileName)
{
    fileNode* currFile = findNode (currDir, fileName);
    if (currFile == NULL || currFile->isDirectory)
    {
        printf("\nFile not exists!");
        return;
    }
    for (int index = 0; index < MAX_BLOCK_PER_FILE; index++)
    {
        if (currFile->blockPointers[index] == -1)
        {
            break;
        }
        freeAllocatedBlock (currFile->blockPointers[index]);
    }
    if (currFile->next == currFile)
    {
        currDir->childHead = NULL;
    }
    else
    {
        currFile->next->previous = currFile->previous;
        currFile->previous->next = currFile->next;
        if (currDir->childHead == currFile)
        {
            currDir->childHead = currFile->next;
        }
    }
    free(currFile);
    printf("\nFile deleted successfullt!");
    return;
}

void rmdirCommand (char* fileName)
{
    fileNode* directory = findNode (currDir, fileName);
    if (directory == NULL || !directory->isDirectory)
    {
        printf("\nDirectory not exists!");
        return;
    }
    if (directory->childHead != NULL)
    {
        printf("\nDirectory not empty!");
        return;
    }

    if (directory->next == directory)
    {
        currDir->childHead = NULL;
    }
    else
    {
        directory->next->previous = directory->previous;
        directory->previous->next = directory->next;
        if (currDir->childHead == directory)
        {
            currDir->childHead = directory->next;
        }
    }
    free(directory);
    printf("\nDirectory delted Successfully!");
    return;
}

void cdCommand (char* fileName)
{
    if (strcmp(fileName, "..") == 0)
    {
        if (currDir->parent != NULL)
        {
            currDir = currDir->parent;
            return;
        }
        else
        {
            printf("\nAlready in root Directory!");
            return;
        }
    }
    fileNode* currFile = findNode(currDir, fileName);
    if (currFile == NULL || !currFile->isDirectory)
    {
        printf("\nDirectory not found!");
        return;
    }
    currDir = currFile;
    printf("\nMoved to  /%s", currDir->fileName);
    return;
}

void lsCommand ()
{
    if (currDir->childHead == NULL)
    {
        printf("\n(empty)");
        return;
    }
    fileNode* temp = currDir->childHead;
    do 
    {
        printf("\n%s%s", temp->fileName, temp->isDirectory ? "/" : "");
        temp = temp->next;
    }while (temp != currDir->childHead);
    return;
}

void pwdCommand ()
{
    char path[100];
    fileNode* temp = currDir;
    while (temp != NULL)
    {
        char tempPath[50];
        strcpy(tempPath, "/");
        strcat(tempPath, temp->fileName);
        strcat(tempPath, path);
        strcpy(path, tempPath);
        temp = temp->parent;
    }
    printf("\n%s", path);
    return;
}

void dfCommand ()
{
    freeBlock* temp = freeBlockHead;
    int freeBlocksCount = 0;
    while (temp != NULL)
    {
        freeBlocksCount++;
        temp = temp->next;
    }
    int used = NUM_BLOCKS - freeBlocksCount;
    float usagePercent = ((float)used / NUM_BLOCKS) * 100;
    printf("\nTotal Blocks: %d\nUsed Blocks: %d\nFree Blocks: %d\nUsage: %.2f%%\n", NUM_BLOCKS, used, freeBlocksCount, usagePercent);
    return;
}

void main ()
{
    initializeFreeBlocks ();
    initializeRootDirectory ();
    printf("\nCompact VFS - ready. Type 'exit' to quit.");
    char command[10], name[50], data[512];
    while (gIsRunning)
    {
        printf("\n%s > ", currDir->fileName);
        scanf("%s", command);

        if (strcmp (command, "mkdir") == 0)
        {
            scanf("%s", name);
            mkdirCommand (name);
        }
        else if (strcmp (command, "create") == 0)
        {
            scanf("%s", name);
            createCommand (name);
        }
        else if (strcmp (command, "delete") == 0)
        {
            scanf("%s", name);
            deleteCommand (name);
        }
        else if (strcmp (command, "write") == 0)
        {
            scanf("%s", name);
            getchar();
            fgets(data, sizeof(data), stdin);
            data[strcspn(data, "\n")] = '\0';
            writeCommand (name, data);
        }
        else if (strcmp (command, "read") == 0)
        {
            scanf("%s", name);
            readCommand (name);
        }
        else if (strcmp (command, "rmdir") == 0)
        {
            scanf("%s", name);
            rmdirCommand (name);
        }
        else if (strcmp (command, "cd") == 0)
        {
            scanf("%s", name);
            cdCommand (name);
        }
        else if (strcmp (command, "ls") == 0)
        {
            lsCommand ();
        }
        else if (strcmp (command, "pwd") == 0)
        {
            pwdCommand ();
        }
        else if (strcmp (command, "df") == 0)
        {
            dfCommand ();
        }
        else if (strcmp (command, "exit") == 0)
        {
            printf("\nMemory released. Exiting...");
            gIsRunning = 0;
            break;
        }
        else
        {
            printf("\nUnknown command!");
        }
    }
    return;
}

