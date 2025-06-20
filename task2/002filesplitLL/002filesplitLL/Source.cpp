#include "Header.h"

Node* createNewNode() {
	Node* p_newNode = new Node;
	p_newNode->chData = new char[ONE_MB*sizeof(char)];
	p_newNode->sizeOfNodeBuffer = 0;
	p_newNode->next = nullptr;
	return p_newNode;
}

void freeNodeList(Node* headOfList) {
	while (headOfList) {
		Node* next = headOfList->next;
		delete[] headOfList->chData;
		delete headOfList;
		headOfList = next;
	}
}

Node* readFileToNodeList(char* fileName) {
	FILE* pFileToList = NULL;
	errno_t fileOpenStatus = fopen_s(&pFileToList, fileName, "rb");
	if (fileOpenStatus != 0) {
		char errorMsg[MAX_LEN];
		strerror_s(errorMsg, sizeof(errorMsg), errno);
		fprintf(stderr, "Error : File opening [%d], %s",
			errno, errorMsg);
		return NULL;
	}

	Node* headOfList = NULL;
	Node* currentNode = NULL;
	Node* tailNodeOfList = NULL;
	char chBuffer[ONE_KB];

	size_t total_bytes = 0;
	size_t node_count = 0;

	while (1) {
		size_t bytes_read = fread_s(chBuffer,ONE_KB, sizeof(char), ONE_KB, pFileToList);
		if (bytes_read == ZERO) {
			if (feof(pFileToList)) break;
			if (ferror(pFileToList)) {
				char errorMsg[ERROR_MESSAGE_MAX_LEN];
				strerror_s(errorMsg, sizeof(errorMsg), errno);
				fprintf_s(stderr, "Error: Reading file '%s' failed [%d] %s\n",
					fileName, errno, errorMsg);
				freeNodeList(headOfList);
				fclose(pFileToList);
				pFileToList = NULL;
				return NULL;
			}
		}

		if (!currentNode || currentNode->sizeOfNodeBuffer == ONE_MB) {
			Node* newNode = createNewNode();
			if (!newNode) {
				freeNodeList(headOfList);
				fclose(pFileToList);
				return NULL;
			}

			if (!headOfList) {
				headOfList = newNode;
				tailNodeOfList = newNode;
			}
			else {
				tailNodeOfList->next = newNode;
				tailNodeOfList = newNode;
			}

			currentNode = newNode;
			node_count++;
		}

		size_t remanining = ONE_MB - currentNode->sizeOfNodeBuffer;
		size_t copy_size = (bytes_read < remanining) ? bytes_read : remanining;
		memcpy(currentNode->chData + currentNode->sizeOfNodeBuffer, chBuffer, copy_size);
		currentNode->sizeOfNodeBuffer += copy_size;
		total_bytes += copy_size;

	}

	fclose(pFileToList);
	pFileToList = NULL;

	printf("Created %zu nodes from %zu bytes.\n", node_count, total_bytes);
	return headOfList;
}

bool writeNodesToFiles(Node* head) {
	Node* pCurrentNode = head;
	size_t nodeCounter = 0;
	char fileName[MAX_FILENAME_LEN];
	memset(fileName, '\0', sizeof(fileName));
	while (pCurrentNode != NULL) {
		snprintf(fileName, MAX_LEN, "../%s%s%zu%s", OUTPUT_DIR, FILE_PRIFIX, nodeCounter++, FILE_SUFFIX);
		FILE* outputFile = NULL;
		errno_t fileOpenStatus = fopen_s(&outputFile, fileName, "wb");
		if (fileOpenStatus != 0) {
			char errorMsg[ERROR_MESSAGE_MAX_LEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to create file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			return false;
		}

		size_t written = fwrite(pCurrentNode->chData, sizeof(char), pCurrentNode->sizeOfNodeBuffer, outputFile);
		if (written != pCurrentNode->sizeOfNodeBuffer) {

			char errorMsg[MAX_LEN];
			strerror_s(errorMsg, sizeof(errorMsg), errno);
			fprintf(stderr, "Error: Failed to write to file '%s' - [%d] %s\n",
				fileName, errno, errorMsg);
			fclose(outputFile);

			return false;
		}

		printf("Saved: %s (%zu bytes)\n", fileName, pCurrentNode->sizeOfNodeBuffer);

		fclose(outputFile);
		pCurrentNode = pCurrentNode->next;
	}

	return true;
}

bool splitFileToMultipleChunks(char* inputFile) {
	Node* pList = readFileToNodeList(inputFile);
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