#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cerrno>
#include <windows.h>

using namespace std;

#define ONEKB 1024
#define ONEMB (ONEKB * ONEKB)
#define FILEPRIFIX "node_"
#define FILESUFFIX ".txt"
#define MAXFILELEN 256
#define MAXERRMSGLEN 512
#define OUTPUTDIR "x64\\Debug\\"


typedef struct Node {
	char* chData;
	struct Node* next;
} Node;	

typedef Node* PNODE;
typedef PNODE* DPNODE;

int TOTALNODE = 0;
PNODE createNewNode();
void freeNodeList(PNODE headOfList);

bool insertNodeAtFirst(DPNODE headOfList, char* chBuffer[]);
bool insertNodeAtLast(DPNODE tailOfList, char* chBuffer[]);

void displayDataOfNode(PNODE);
void displayDataOfNNode(PNODE, int nodeIndex);

PNODE readFileToNodeList(char* fileName);
bool writeNodesToFiles(PNODE head);
bool splitFileToMultipleChunks(char* inputFile);

