# VSI-C-CPP


### Task 2 
```
splits a large input file into multiple smaller files, each of size up to 1 MB, using a linked list to store chunks in memory temporarily.
```

### HIGH-LEVEL FLOW:
* main() starts, checks for command-line input (the file to split).

* Calls splitFileToMultipleChunks(inputFile):

    * Reads the file in 1 KB chunks into a linked list of Nodes (readFileToNodeList) 
    * Each node stores up to 1 MB
    * Writes each Node to a separate file (writeNodesToFiles)
    * Cleans up memory
* Program ends.

#### splitFileToMultipleChunks(char* inputFile)
* Calls readFileToNodeList() → loads the file into a linked list of chunks.

* Calls writeNodesToFiles() → writes each node into a file node_0.txt, node_1.txt, etc.

* Frees memory with freeNodeList()

#### readFileToNodeList(char* fileName)
* Opens input file in binary read mode (fopen_s)
* Reads 1 KB (ONE_KB) at a time into chBuffer
* Copies data into the current node

    * If current node is full (1 MB), creates a new one using createNewNode()
* Keeps track of:
    
    * node_count: How many 1 MB chunks
    * total_bytes: Total read
* Returns head of the linked list

#### writeNodesToFiles(Node* head)
* Iterates through the linked list

* For each node:

   * Generates a file name node_<number>.txt

   * Opens the file for binary writing (fopen_s)

   * Writes the node’s data using fwrite

   * Closes the file

* Returns true on success, false on failure