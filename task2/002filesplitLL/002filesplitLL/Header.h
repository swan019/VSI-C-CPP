#pragma once
#include <string>

using namespace std;

#define ONE_KB 1024            
#define ONE_MB   (1024 * ONE_KB) 
#define FILE_PRIFIX "node_"
#define FILE_SUFFIX ".txt"
#define MAX_FILENAME_LEN 256
#define MAX_LEN 256

typedef struct Node {
	char* data;				//pointer to point 1MB data block
	size_t size;			//current data size in this block
	struct Node* next;		//pointer to next node
} Node;

Node* createNewNode();
void freeNodeList(Node* head);
Node* readFileToNodeList(char* fileName);
bool writeNodesToFiles(Node* head);
bool splitFileToMultipleChunks(char* inputFile);