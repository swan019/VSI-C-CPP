#pragma once
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

using namespace std;

#define ONEKB 1024
#define ONEMB (ONEKB * ONEKB)
#define FILEPRIFIX "node_"
#define FILESUFFIX ".txt"
#define MAXFILELEN 256
#define MAXERRMSGLEN 512
#define OUTPUTDIR "x64\\Debug\\"

typedef struct Node
{
	char* chData;
	struct Node* next;
} Node;

typedef Node* PNODE;
typedef PNODE* DPNODE;

int TOTALNODE = 0;
PNODE CreateNewNode();
void FreeNodeList(PNODE headOfList);

bool InsertNodeAtFirst(DPNODE headOfList, char* chBuffer[]);
bool InsertNodeAtLast(DPNODE tailOfList, char* chBuffer[]);

PNODE ReadFileToNodeList(char* fileName);
bool WriteNodesToFiles(PNODE head);
bool SplitFileToMultipleChunks(char* inputFile);
