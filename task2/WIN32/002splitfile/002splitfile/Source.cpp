#include "Header.h"

PNODE CreateNewNode()
{
	PNODE pNode = (PNODE)malloc(sizeof(Node));
	
	if (NULL == pNode)
	{
		fprintf(stderr, "Error : failed to allocate memory for Node.\n");
		return NULL;
	}

	pNode->chData = (char*)malloc(sizeof(char) * (ONEMB + 1));
	if (NULL == pNode->chData)
	{
		fprintf(stderr, "Error : Failed to allocate memory for chData.\n");
		free(pNode);
		return NULL;
	}
	pNode->next = NULL;
	return pNode;
}

void FreeNodeList(PNODE pHeadOfList) {
	while (pHeadOfList)
	{
		PNODE next = pHeadOfList->next;
		free(pHeadOfList->chData);
		free(pHeadOfList);
		pHeadOfList = next;
	}
}

bool InsertNodeAtFirst(DPNODE pHeadOfList, char chBuffer[])
{
	PNODE newNode = CreateNewNode();
	if (newNode == NULL)
	{

		return false;
	}
	memcpy(newNode->chData, chBuffer, ONEMB);
	newNode->next = *pHeadOfList;
	*pHeadOfList = newNode;

	printf("Inserted node at the front of the linked list successfully.\n");
	return true;
}

bool InsertNodeAtLast(DPNODE pHeadOfList, DPNODE pTailOfList, char chBuffer[])
{
	PNODE newNode = CreateNewNode();
	if (newNode == NULL)
	{
		return false;
	}

	memcpy(newNode->chData, chBuffer, ONEMB);
	newNode->chData[ONEMB] = '\0';

	if (*pHeadOfList == NULL)
	{
		*pHeadOfList = *pTailOfList = newNode;
	}
	else
	{
		(*pTailOfList)->next = newNode;
		*pTailOfList = newNode;
	}

	printf("Inserted node at the end of the linked list successfully.\n");
	return true;
}

PNODE ReadFileToNodeList(char* fileName)
{
	HANDLE hFileToList = CreateFileA(
		fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (INVALID_HANDLE_VALUE == hFileToList)
	{
		fprintf(stderr, "CreateFile failed (%lu)\n", GetLastError());
		return NULL;
	}

	PNODE pHeadOfList = NULL;
	PNODE pTailOfList = NULL;

	int countOfListNode = 0;
	char* pChBuffer = (char*)malloc(ONEMB * sizeof(char));
	if (NULL == pChBuffer)
	{
		fprintf(stderr, "Error: chBuffer Memory allocation failed.\n");
		return NULL;
	}
	memset(pChBuffer, 0, ONEMB);

	DWORD bytesRead = 0;
	BOOL readResult;

	while (true)
	{
		readResult = ReadFile(hFileToList, pChBuffer, ONEMB, &bytesRead, NULL);
		if (FALSE == readResult) 
		{
			DWORD ErrorMsg = GetLastError();
			fprintf(stderr, "Error: ReadFile failed [%lu]\n", ErrorMsg);
			FreeNodeList(pHeadOfList);
			free(pChBuffer);
			return NULL;
		}

		if (bytesRead == 0)
			break;

		bool StatusOfNodeInsert = InsertNodeAtLast(&pHeadOfList, &pTailOfList, pChBuffer);
		if (!StatusOfNodeInsert)
		{
			fprintf(stderr, "Error: Insert node at last failed.\n");
			FreeNodeList(pHeadOfList);
			free(pChBuffer);
			CloseHandle(hFileToList);
			return NULL;
		}

		countOfListNode += 1;

	}

	CloseHandle(hFileToList);
	free(pChBuffer);
	pChBuffer = NULL;
	printf("Created %d nodes.\n", countOfListNode);
	return pHeadOfList;
}

bool WriteNodeToFiles(PNODE pHeadOfNode)
{
	PNODE pCurrentNode = pHeadOfNode;
	size_t nodeCounter = 0;
	char fileName[MAXFILELEN];
	memset(fileName, '\0', sizeof(fileName));

	while (pCurrentNode != NULL)
	{
		snprintf(fileName, MAXFILELEN, "../%s%s%zu%s", OUTPUTDIR, FILEPRIFIX, nodeCounter++, FILESUFFIX);
		HANDLE hFile = CreateFileA(
			fileName,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			DWORD err = GetLastError();
			fprintf(stderr, "Error: Failed to create file '%s' - [%lu]\n", fileName, err);
			return false;
		}

		DWORD bytesWritten = 0;
		BOOL writeSuccess = WriteFile(
			hFile,
			pCurrentNode->chData,
			ONEMB,
			&bytesWritten,
			NULL
		);

		if (!writeSuccess || bytesWritten != ONEMB)
		{
			DWORD ErrorMsg = GetLastError();
			fprintf(stderr, "Error: Failed to write to file '%s' - [%lu]\n", fileName, ErrorMsg);
			CloseHandle(hFile);
			return false;
		}
		printf("Saved: %s (%d bytes)\n", fileName, ONEMB);
		CloseHandle(hFile);
		pCurrentNode = pCurrentNode->next;
	}

	return true;
}

bool SplitFileToMultipleChunks(char* pChInputFile)
{
	PNODE pList = ReadFileToNodeList(pChInputFile);
	if (NULL == pList)
	{
		fprintf(stderr, "Error: Failed to convert file to linked list.\n");
		return false;
	}

	bool IsWriteSuccessful = WriteNodeToFiles(pList);

	if (!IsWriteSuccessful)
	{
		fprintf(stderr, "Error: Failed to write nodes to output.\n");
	}

	FreeNodeList(pList);
	return IsWriteSuccessful;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <output_file_name>\n", argv[0]);
		return 1;
	}

	char* pChInputFile = argv[1];
	bool isFileSplit = SplitFileToMultipleChunks(pChInputFile);
	if (isFileSplit == false)
	{
		fprintf(stderr, "Error: File generation failed for '%s'.\n", pChInputFile);
		return 1;
	}

	fprintf(stdout, "Info: File '%s' generated successfully.\n", pChInputFile);
	return 0;
}