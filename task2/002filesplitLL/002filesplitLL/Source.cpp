#include "Header.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <windows.h>

Node* createNewNode() {
    Node* p_newNode = new Node;
    p_newNode->data = new char[ONE_MB];
    p_newNode->size = 0;
    p_newNode->next = nullptr;
    return p_newNode;
}

void freeNodeList(Node* head) {
    while (head) {
        Node* next = head->next;
        delete[] head->data;
        delete head;
        head = next;
    }
}

Node* readFileToNodeList(char* fileName) {
	FILE* file = NULL;
	errno_t err = fopen_s(&file, fileName, "rb");
	if (err != 0 || file == NULL) {
		char errorMsg[MAX_LEN];
		strerror_s(errorMsg, sizeof(errorMsg), errno);
		fprintf(stderr, "Error : File opening [%d], %s",
			errno, errorMsg);
		return NULL;
	}

	Node* head = NULL;
	Node* current = NULL;
	char chBuffer[ONE_KB];
	size_t total_bytes = 0;
	size_t node_count = 0;

	while (1) {
		size_t bytes_read = fread(chBuffer, sizeof(char), ONE_KB, file);
		if (bytes_read == 0) {
			if (feof(file)) break;
			if (ferror(file)) {
				char errorMsg[MAX_LEN];
				strerror_s(errorMsg, sizeof(errorMsg), errno);
				fprintf(stderr, "Error: Reading file '%s' failed [%d] %s\n",
					fileName, errno, errorMsg);
				freeNodeList(head);
				fclose(file);
				return NULL;
			}
		}

		if (!current || current->size == ONE_MB) {
			Node* newNode = createNewNode();
			if (!newNode) {
				freeNodeList(head);
				fclose(file);
				return NULL;
			}

			if (!head) {
				head = newNode;
			}
			else {
				Node* last = head;
				while (last->next != NULL) {
					last = last->next;
				}
				last->next = newNode;
			}

			current = newNode;
			node_count++;
		}

		//calculate copy amount
		size_t remanining = ONE_MB - current->size;
		size_t copy_size = (bytes_read < remanining) ? bytes_read : remanining;

		//copy to current node
		memcpy(current->data + current->size, chBuffer, copy_size);
		current->size += copy_size;
		total_bytes += copy_size;

	}

	fclose(file);
	printf("Created %zu nodes from %zu bytes.\n", node_count, total_bytes);
	return head;
}

bool writeNodesToFiles(Node* head) {
	Node* p_currentNode = head;
	size_t nodeCounter = 0;
	char fileName[MAX_FILENAME_LEN];

	while (p_currentNode != NULL) {

		snprintf(fileName, MAX_FILENAME_LEN, "%s%zu%s", FILE_PRIFIX, nodeCounter++, FILE_SUFFIX);

		FILE* outputFile = NULL;
		errno_t err = fopen_s(&outputFile, fileName, "wb");
		if (err != 0 ||  outputFile == NULL) {
			char errorMsg[MAX_LEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to create file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			return false;
		}

		size_t written = fwrite(p_currentNode->data, 1, p_currentNode->size, outputFile);
		if (written != p_currentNode->size) {

			char errorMsg[MAX_LEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to write to file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			fclose(outputFile);

			return false;
		}

		printf("Saved: %s (%zu bytes)\n", fileName, p_currentNode->size);

		fclose(outputFile);
		p_currentNode = p_currentNode->next;
	}

	return true;
}

bool splitFileToMultipleChunks(char* inputFile) {
	Node* p_list = readFileToNodeList(inputFile);
	if (!p_list) {
		fprintf(stderr, "Error: Failed to convert file to linked list.\n");
		return false;
	}

	bool success = writeNodesToFiles(p_list);
	if (!success) {
		fprintf(stderr, "Error: Failed to write nodes to output.\n");
	}

	freeNodeList(p_list);
	return success;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <output_file_name>\n", argv[0]);
		return 1;
	}

	char* inputFile = argv[1];

	bool isFileSplit = splitFileToMultipleChunks(inputFile);
	if (isFileSplit == false) {
		fprintf(stderr, "Error: File generation failed for '%s'.\n", inputFile);
		return 1;
	}
	fprintf(stdout, "Info: File '%s' generated successfully.\n", inputFile);
	return 0;
}