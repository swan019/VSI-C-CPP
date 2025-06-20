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
#define MAXLEN 256
#define OUTPUTDIR "x64\\Debug\\"

typedef struct Node {
	char* chData;
	struct Node* next;
} Node;	

typedef Node* PNODE;

int TOTALNODE = 0;
PNODE createNewNode();
void freeNodeList(PNODE headOfList);

bool insertNodeAtFirst(PNODE headOfList, PNODE currentNewNode);
bool insertNodeAtLast(PNODE tailOfList, PNODE currentNewNode);

void displayDataOfNode(PNODE);
void displayDataOfNNode(PNODE, int nodeIndex);

PNODE readFileToNodeList(char* fileName);
bool writeNodesToFiles(PNODE head);
bool splitFileToMultipleChunks(char* inputFile);

