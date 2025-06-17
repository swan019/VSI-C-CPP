#ifndef LIST_UTILS_HPP
#define LIST_UTILS_HPP

#include <string>

using namespace std;

#define BUFFER_SIZE 1024            
#define NODE_SIZE   (1024 * BUFFER_SIZE) 
#define NODE_FILE_PRIFIX "node_"
#define NODE_FILE_SUFFIX ".txt"

struct Node {
    char* data;
    size_t size;
    Node* next;
};

Node* createNode();
void freeList(Node* head);
Node* fileToLinkedList(string& fileName);
bool writeAllNodes(Node* head);
bool splitFile(string& inputFile);

#endif
