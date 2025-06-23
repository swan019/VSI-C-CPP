#include "Header.h"

PNODE createNewNode()
{
    PNODE pNode = (PNODE)malloc(sizeof(Node));
    if (!pNode)
    {
        fprintf(stderr, "Error: Failed to allocate memory for Node.\n");
        return NULL;
    }
    pNode->chData = (char*)malloc((ONEMB + 1) * sizeof(char));
    if (!pNode->chData)
    {
        fprintf(stderr, "Error: Failed to allocate memory for chData.\n");
        free(pNode);
        return NULL;
    }
    pNode->next = NULL;
    return pNode;
}

void freeNodeList(PNODE headOfList)
{
    while (headOfList)
    {
        PNODE next = headOfList->next;
        free(headOfList->chData);
        free(headOfList);
        headOfList = next;
    }
}


void displayEachNodeChData(PNODE headOfList)
{
    PNODE currentNode = headOfList;
    while (currentNode != NULL)
    {
        printf("%c -> ", currentNode->chData[0]);
        currentNode = currentNode->next;
    }
    printf("\n");
}

void displayDataOfNNode(PNODE headOfList, int nodeIndex)
{
    int currentNodeIndex = 0;
    PNODE currentNode = headOfList;
    while (currentNode && currentNodeIndex < nodeIndex)
    {
        currentNode = currentNode->next;
        currentNodeIndex++;
    }
    if (currentNode == NULL)
    {
        fprintf(stderr, "Error : Node index %d not valid. \n", currentNodeIndex);
        return;
    }
    printf("Node contains %d byte : \n", ONEMB);
    printf("%s", currentNode->chData);
    printf("\n");
}

bool insertNodeAtFirst(DPNODE headOfList, char chBuffer[])
{
    PNODE newNode = createNewNode();
    if (newNode == NULL)
    {

        return false;
    }
    memcpy(newNode->chData, chBuffer, ONEMB);
    newNode->next = *headOfList;
    *headOfList = newNode;

    printf("Inserted node at the front of the linked list successfully.\n");
    return true;
}

bool insertNodeAtLast(DPNODE headOfList, DPNODE tailOfList, char chBuffer[])
{
    PNODE newNode = createNewNode();
    if (newNode == NULL)
    {
        return false;
    }

    memcpy(newNode->chData, chBuffer, ONEMB);
    newNode->chData[ONEMB] = '\0';

    if (*headOfList == NULL)
    {
        *headOfList = *tailOfList = newNode;
    }
    else
    {
        (*tailOfList)->next = newNode;
        *tailOfList = newNode;
    }

    printf("Inserted node at the end of the linked list successfully.\n");
    return true;
}

PNODE readFileToNodeList(char* fileName)
{
    FILE* pFileToList = NULL;
    errno_t fileOpenStatus = fopen_s(&pFileToList, fileName, "rb");

    if (fileOpenStatus != 0)
    {
        char errMsg[MAXERRMSGLEN];
        strerror_s(errMsg, sizeof(errMsg), errno);
        fprintf(stderr, "Error : File opening [%d], %s",
            errno, errMsg);
        return NULL;
    }

    PNODE headOfList    =  NULL;
    PNODE currentNode   =  NULL;
    PNODE tailOfList    =  NULL;

    int countOfListNode = 0;
    char* chBuffer      = (char*)malloc(ONEMB * sizeof(char));
    if (!chBuffer)
    {
        fprintf(stderr, "Error: chBuffer Memory allocation failed.\n");
        return NULL;
    }
    memset(chBuffer, 0, ONEMB);
    while (true)
    {
        size_t bytesReadFromFile = fread_s(chBuffer, ONEMB, sizeof(char), ONEMB, pFileToList);
        if (bytesReadFromFile == 0)
        {
            if (feof(pFileToList))
                break;
            if (ferror(pFileToList))
            {
                char errorMsg[MAXERRMSGLEN];
                strerror_s(errorMsg, sizeof(errorMsg), errno);
                fprintf_s(stderr, "Error: Reading file '%s' failed [%d] %s\n", fileName, errno, errorMsg);
                freeNodeList(headOfList);
                fclose(pFileToList);
                pFileToList = NULL;
                return NULL;
            }
        }
        else
        {
            bool status = insertNodeAtLast(&headOfList, &tailOfList, chBuffer);
            if (!status)
            {
                fprintf(stderr, "Error: Insert node at last failed.\n");
                freeNodeList(headOfList);
                fclose(pFileToList);
                return NULL;
            }

            countOfListNode += 1;
        }

        //if (bytesReadFromFile < ONEMB)
        //    break;
    }

    fclose(pFileToList);
    free(chBuffer);
    chBuffer = NULL;
    printf("Created %d nodes.\n", countOfListNode);
    return headOfList;
}

bool writeNodesToFiles(PNODE head)
{
    PNODE pCurrentNode = head;
    size_t nodeCounter = 0;
    char fileName[MAXFILELEN];
    memset(fileName, '\0', sizeof(fileName));
    while (pCurrentNode != NULL)
    {
        snprintf(fileName, MAXFILELEN, "../%s%s%zu%s", OUTPUTDIR, FILEPRIFIX, nodeCounter++, FILESUFFIX);
        FILE* outputFile = NULL;
        errno_t fileOpenStatus = fopen_s(&outputFile, fileName, "wb");
        if (fileOpenStatus != 0)
        {
            char errorMsg[MAXERRMSGLEN];
            strerror_s(errorMsg, sizeof(errorMsg), errno);
            fprintf(stderr, "Error: Failed to create file '%s' - [%d] %s\n",
                fileName, errno, errorMsg);
            return false;
        }
        size_t written = fwrite(pCurrentNode->chData, sizeof(char), ONEMB, outputFile);
        if (written != ONEMB)
        {
            char errorMsg[MAXERRMSGLEN];
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

bool splitFileToMultipleChunks(char* pchInputFile)
{
    PNODE pList = readFileToNodeList(pchInputFile);

    if (!pList)
    {
        fprintf(stderr, "Error: Failed to convert file to linked list.\n");
        return false;
    }

    displayEachNodeChData(pList);

    int nodeNo;
    printf("Enter which Node Data want to see : ");
    scanf_s("%d", &nodeNo);

    displayDataOfNNode(pList, nodeNo);

    bool isWriteSuccessful = writeNodesToFiles(pList);

    if (!isWriteSuccessful)
    {
        fprintf(stderr, "Error: Failed to write nodes to output.\n");
    }

    freeNodeList(pList);
    return isWriteSuccessful;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <output_file_name>\n", argv[0]);
        return 1;
    }

    char* pchInputFile = argv[1];

    bool isFileSplit = splitFileToMultipleChunks(pchInputFile);

    if (isFileSplit == false)
    {
        fprintf(stderr, "Error: File generation failed for '%s'.\n", pchInputFile);
        return 1;
    }

    fprintf(stdout, "Info: File '%s' generated successfully.\n", pchInputFile);
    return 0;
}