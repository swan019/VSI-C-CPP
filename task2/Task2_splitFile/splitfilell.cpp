#include "splitfilell.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdio>

Node* createNode() {
    Node* p_newNode = new Node;
    p_newNode->data = new char[NODE_SIZE];
    p_newNode->size = 0;
    p_newNode->next = nullptr;
    return p_newNode;
}

void freeList(Node* head) {
    while (head) {
        Node* next = head->next;
        delete[] head->data;
        delete head;
        head = next;
    }
}

Node* fileToLinkedList(string& fileName) {
    ifstream inFile(fileName, ios::binary);
    if (!inFile) {
        fprintf(stderr, "Error: Failed to open input file '%s' - [%d] %s\n",
            fileName.c_str(), errno, strerror(errno));
        return nullptr;
    }

    Node* head = nullptr;
    Node* current = nullptr;
    char buffer[BUFFER_SIZE];
    size_t totalBytes = 0, nodeCount = 0;

    while (inFile) {
        inFile.read(buffer, BUFFER_SIZE);
        size_t bytesRead = inFile.gcount();
        if (bytesRead == 0) break;

        // Allocate new node if necessary
        if (!current || current->size == NODE_SIZE) {
            Node* newNode = createNode();
            if (!newNode) {
                fprintf(stderr, "Error: Failed to allocate memory for new node.\n");
                freeList(head);
                return nullptr;
            }

            if (head == nullptr)
                head = newNode;
            else
                current->next = newNode;

            current = newNode;
            nodeCount++;
        }

        // Copy data into current node
        size_t remaining = NODE_SIZE - current->size;
        size_t copySize = min(bytesRead, remaining);

        memcpy(current->data + current->size, buffer, copySize);
        current->size += copySize;
        totalBytes += copySize;
    }

    printf("Created %zu nodes from %zu bytes.\n", nodeCount, totalBytes);
    return head;
}

bool writeAllNodes(Node* head) {
    Node* p_currentNode = head;
    size_t nodeCounter = 0;

    while (p_currentNode != nullptr) {
        string fileName = NODE_FILE_PRIFIX + to_string(nodeCounter++) + NODE_FILE_SUFFIX;
        ofstream outFile(fileName, ios::binary);
        if (!outFile) {
            fprintf(stderr, "Error: Failed to create file '%s' - [%d] %s\n",
                fileName.c_str(), errno, strerror(errno));
            return false;
        }

        outFile.write(p_currentNode->data, p_currentNode->size);
        if (!outFile) {
            fprintf(stderr, "Error: Failed to write to file '%s' - [%d] %s\n",
                fileName.c_str(), errno, strerror(errno));
            return false;
        }

        printf("Saved: %s (%zu bytes)\n", fileName.c_str(), p_currentNode->size);
        p_currentNode = p_currentNode->next;
    }

    return true;
}

bool splitFile(string& inputFile) {
    Node* p_list = fileToLinkedList(inputFile);
    if (!p_list) {
        fprintf(stderr, "Error: Failed to convert file to linked list.\n");
        return false;
    }

    bool success = writeAllNodes(p_list);
    if (!success) {
        fprintf(stderr, "Error: Failed to write nodes to output.\n");
    }

    freeList(p_list);
    return success;
}
