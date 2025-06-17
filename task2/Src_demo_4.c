#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define KB 1024
#define MB (1024 * KB)
#define NODESIZE (1 * MB) //Each node hold 1MB
#define OUTPUT_PREFIX "node_"
#define OUTPUT_EXTENTION ".txt"
#define BUFFERSIZE (1 * KB)
#define FILEMAXLEN 256


typedef struct Node {
	char* data;				//pointer to point 1MB data block
	size_t size;			//current data size in this block
	struct Node* next;		//pointer to next node
} Node;


//Create new empty Node
Node* CreateNode() {
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (!newNode) {
		fprintf(stderr, "Error : Failed to create new node, [%d] %s.\n",
				errno, strerror(errno));
		return NULL;
	}

	newNode->data = (char*)malloc(sizeof(char) * MB);
	if (!newNode->data) {
		free(newNode);
		return NULL;
	}

	newNode->size = 0;
	newNode->next = NULL;
	return newNode;
}


void freeNode(Node* head) {
	Node* nextNode = NULL;
	while (head) {
		nextNode = head->next;
		free(head->data);
		free(head);
		head = nextNode;
	}
}

// Read file and build node list
Node* FileToLinkedList(char* fileName) {
	FILE* file = fopen(fileName, "rb");
	if (!file) {
		fprintf(stderr, "Error : File opening [%d], %s",
				errno, strerror(errno));
		return NULL;
	}

	Node* head = NULL;
	Node* current = NULL;
	char buffer[BUFFERSIZE];
	size_t total_bytes = 0;
	size_t node_count = 0;

	while (1) {
		//read 1KB from file
		size_t bytes_read = fread(buffer, sizeof(char), BUFFERSIZE, file);
		if (bytes_read == 0) {
			if (feof(file)) break;
			if (ferror(file)) {
				fprintf(stderr, "Error: Reading file '%s' failed [%d] %s\n", 
						fileName, errno, strerror(errno));
				freeNode(head);
				fclose(file);
				return NULL;
			}
		}

		//create new Node if needed
		if (!current || current->size == NODESIZE) {
			Node* newNode = CreateNode();
			if (!newNode) {
				freeNode(head);
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
		size_t remanining = NODESIZE - current->size;
		size_t copy_size = (bytes_read < remanining) ? bytes_read : remanining;

		//copy to current node
		memcpy(current->data + current->size, buffer, copy_size);
		current->size += copy_size;
		total_bytes += copy_size;

	}

	fclose(file);
	printf("Created %zu nodes from %zu bytes.\n", node_count, total_bytes);
	return head;
}

bool WriteNodeData(char* data, size_t size, size_t Node_Counter) {
	char fileName[FILEMAXLEN];

	snprintf(fileName, sizeof(fileName), 
			"%s%zu%s", OUTPUT_PREFIX, Node_Counter, OUTPUT_EXTENTION);
	
	FILE* file = fopen(fileName, "w");

	if (!file) {
		fprintf(stderr, "Error : File opening [%d], %s",
				errno, strerror(errno));
		return false;
	}

	size_t written = fwrite(data, sizeof(char), size, file);
	if (written != MB) {
		fprintf(stderr, "Error: Failed to write to file at %dMB block [%d] %s\n",
			Node_Counter, errno, strerror(errno));
		fclose(file);
		file = NULL;
		return false;
	}

	fclose(file);
	file = NULL;
	printf("Saved %s (%zu bytes)\n", fileName, size);
	return true;
}


// Write all nodes to files
bool Write_All_Nodes(Node* head) {
	Node* current = head;
	size_t Node_Counter = 0;
	while (current) {
		bool isNodeDataWriteInFile = WriteNodeData(current->data, current->size, Node_Counter);
		if (!isNodeDataWriteInFile) {
			return false;
		}
		current = current->next;
		Node_Counter++;
	}
	return true;
}


int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage :%s <FileName>", argv[0]);
		return 1;
	}

	char* fileName = argv[1];
	Node* list = FileToLinkedList(fileName);
	if (!list) {
		return 1;
	}

	bool isFileSplit = Write_All_Nodes(list);
	if (!Write_All_Nodes) {
		freeNode(list);
		return 1;
	}

	freeNode(list);
	return 0;
}