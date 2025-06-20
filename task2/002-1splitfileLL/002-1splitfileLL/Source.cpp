#include "Header.h"

PNODE createNewNode() {
	PNODE pNode = new Node;
	pNode->chData = new char[ONEMB * sizeof(char)];
	pNode->next = NULL;
	return pNode;
}

void freeNodeList(PNODE headOfList) {
	while (headOfList) {
		PNODE next = headOfList->next;
		delete[] headOfList->chData;
		delete headOfList;
		headOfList = next;
	}
}

void displayDataOfNode(PNODE nodeOfList) {
	if (nodeOfList == NULL) {
		fprintf(stderr, "Error : Node is NULL.\n");
		return;
	}
	printf("Node contains %d byte : \n", ONEMB);
	for (size_t i = 0; i < ONEMB; i++) {
		printf("%c", nodeOfList->chData[i]);
	}
	printf("\n");
	return;
}

void displayEachNodeChData(PNODE headOfList) {
	PNODE currentNode = headOfList;
	while (currentNode != NULL) {
		printf("%c -> ", currentNode->chData[0]);
		currentNode = currentNode->next;
	}
	printf("\n");
}

void displayDataOfNNode(PNODE headOfList, int nodeIndex) {
	int currentNodeIndex = 0;
	PNODE currentNode = headOfList;
	while (currentNode && currentNodeIndex < nodeIndex) {
		currentNode = currentNode->next;
		currentNodeIndex++;
	}
	if (currentNode == NULL) {
		fprintf(stderr, "Error : Node index %d not valid. \n", currentNodeIndex);
		return;
	}
	printf("Node contains %d byte : \n", ONEMB);
	for (size_t i = 0; i < ONEMB; i++) {
		printf("%c", currentNode->chData[i]);
	}
	printf("\n");
}

bool insertNodeAtFirst(PNODE* headOfList, PNODE currentNewNode) {
	if (headOfList == NULL) {
		fprintf(stderr, "Error: headOfList is NULL. Cannot insert at the beginning.\n");
		return false;
	}
	if (currentNewNode == NULL) {
		fprintf(stderr, "Error: currentNewNode is NULL. Cannot insert an empty node.\n");
		return false;
	}
	currentNewNode->next = *headOfList;
	*headOfList = currentNewNode;
	printf("Inserted node at the front of the linked list successfully.\n");
	return true;
}

bool insertNodeAtLast(PNODE* tailOfList, PNODE currentNewNode) {
	if (tailOfList == NULL) {
		fprintf(stderr, "Error: tailOfList is NULL. Cannot insert at the end.\n");
		return false;
	}
	if (currentNewNode == NULL) {
		fprintf(stderr, "Error: currentNewNode is NULL. Cannot insert an empty node.\n");
		return false;
	}
	(*tailOfList)->next = currentNewNode;
	*tailOfList = currentNewNode;
	printf("Inserted node at the end of the linked list successfully.\n");
	return true;
}




PNODE readFileToNodeList(char* fileName) {
	FILE* pFileToList = NULL;
	errno_t fileOpenStatus = fopen_s(&pFileToList, fileName, "rb");
	if (fileOpenStatus != 0) {
		char errMsg[MAXLEN];
		strerror_s(errMsg, sizeof(errMsg), errno);
		fprintf(stderr, "Error : File opening [%d], %s",
				errno, errMsg);
		return NULL;
	}

	PNODE headOfList = NULL;
	PNODE currentNode = NULL;
	PNODE tailOfList = NULL;
	int countOfListNode = 0;

	while (true) {
		PNODE newNode = createNewNode();
		if (newNode == NULL) {
			freeNodeList(headOfList);
			fclose(pFileToList);
			return NULL;
		}

		size_t bytesReadFromFile = fread_s(newNode->chData, ONEMB, sizeof(char), ONEMB, pFileToList);
		if (bytesReadFromFile == 0) {

			delete[] newNode->chData;
			delete newNode;

			if (feof(pFileToList)) break;
			if (ferror(pFileToList)) {
				char errorMsg[MAXLEN];
				strerror_s(errorMsg, sizeof(errorMsg), errno);
				fprintf_s(stderr, "Error: Reading file '%s' failed [%d] %s\n",
						fileName, errno, errorMsg);
				freeNodeList(headOfList);
				fclose(pFileToList);
				pFileToList = NULL;
				return NULL;
			}
		}
		else {
			if (headOfList == NULL) {
				bool isNodeInsertAtFirst = insertNodeAtFirst(&headOfList, newNode);
				if (isNodeInsertAtFirst == false) {
					freeNodeList(headOfList);
					fclose(pFileToList);
					fprintf(stderr, "Error : New node not insert at begining.\n");
					return NULL;
				}
				tailOfList = newNode;
			}
			else {
				bool isNodeInsertAtLast = insertNodeAtLast(&tailOfList, newNode);
				if (isNodeInsertAtLast == false) {
					freeNodeList(headOfList);
					fclose(pFileToList);
					fprintf(stderr, "Error : New node not insert at end.\n");
					return NULL;
				}
			}
			countOfListNode += 1;
		}
		if (bytesReadFromFile < ONEMB) break;
	}

	fclose(pFileToList);
	printf("Created %d nodes.\n", countOfListNode);
	return headOfList;
}

bool writeNodesToFiles(PNODE head) {
	PNODE pCurrentNode = head;
	size_t nodeCounter = 0;
	char fileName[MAXLEN];
	memset(fileName, '\0', sizeof(fileName));
	while (pCurrentNode != NULL) {
		snprintf(fileName, MAXLEN, "../%s%s%zu%s", OUTPUTDIR, FILEPRIFIX, nodeCounter++, FILESUFFIX);
		FILE* outputFile = NULL;
		errno_t fileOpenStatus = fopen_s(&outputFile, fileName, "wb");
		if (fileOpenStatus != 0) {
			char errorMsg[MAXLEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to create file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			return false;
		}
		size_t written = fwrite(pCurrentNode->chData, sizeof(char), ONEMB, outputFile);
		if (written != ONEMB) {
			char errorMsg[MAXLEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to write to file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			fclose(outputFile);

			return false;
		}
		printf("Saved: %s (%d bytes)\n", fileName, ONEMB);

		fclose(outputFile);
		pCurrentNode = pCurrentNode->next;
	}

	return true;
}

bool splitFileToMultipleChunks(char* inputFile) {
	PNODE pList = readFileToNodeList(inputFile);

	displayEachNodeChData(pList);

	if (!pList) {
		fprintf(stderr, "Error: Failed to convert file to linked list.\n");
		return false;
	}

	bool isWriteSuccessful = writeNodesToFiles(pList);

	if (!isWriteSuccessful) {
		fprintf(stderr, "Error: Failed to write nodes to output.\n");
	}

	freeNodeList(pList);
	return isWriteSuccessful;
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