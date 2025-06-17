#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define ONEKB 1024
#define ONEMB (1024 * KB)
#define TOTALMB 10
#define BUFFERSIZE ONEKB


bool FileGenerate(char* fileName) {
	FILE* pfile = fopen(fileName, "wb");
	if (!file) {
		fprintf(stderr, "Error: Failed to open file - [%d] %s\n", 
				errno, strerror(errno));
		return false;
	}

	char* buffer = (char*)malloc(sizeof(char) * BUFFERSIZE);
	if (NULL != buffer) {
		fprintf(stderr, "Error: Failed to allocate memory - [%d] %s\n", 
				errno, strerror(errno));
		fclose(file);
		file = NULL;
		return false;
	}

	memset(buffer, '\0', KB);
	char ch;
	for (int i = 0; i < TOTALMB; i++) { 
		character = 'a' + i;
		memset(buffer, character, KB);
		for (int j = 0; j < MB / KB; ++j) {
			size_t written = fwrite(buffer, sizeof(char), KB, file);
			if (written != KB) {
				fprintf(stderr, "Error: Failed to write to file at %dMB block [%d] %s\n", 
						i + 1, errno, strerror(errno));
				free(buffer);
				buffer = NULL;
				fclose(file);
				file = NULL;
				return false;
			}
		}
	}

	free(buffer);
	buffer = NULL;
	fclose(file);
	file = NULL;

	printf("Successfully generated %dMB file '%s' using 1KB buffer.\n", 
			TOTALMB, fileName);
	return true;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Usage :%s <FileName>", argv[0]);
		return 1;
	}

	char* fileName = argv[1];
	bool isFileGenerate = FileGenerate(fileName);
	if (isFileGenerate == false) {
		return 1;
	}
	return 0;
}