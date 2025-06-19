#pragma once
#include <string>

using namespace std;

#define ONE_KB 1024            
#define ONE_MB   (1024 * ONE_KB) 
#define FILE_PRIFIX "node_"
#define FILE_SUFFIX ".txt"
#define MAX_FILENAME_LEN 256
#define MAX_LEN 256
#define ERROR_MESSAGE_MAX_LEN 256
#define OUTPUT_DIR "x64\\Debug\\"
#define ZERO 0


typedef struct Node {
	char* chData;				
	size_t sizeOfNodeBuffer;
	struct Node* next;		
} Node;

Node* createNewNode();
void freeNodeList(Node* head);
Node* readFileToNodeList(char* fileName);
bool writeNodesToFiles(Node* head);
bool splitFileToMultipleChunks(char* inputFile); 
