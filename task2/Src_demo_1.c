#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define TENB 10 // 10 Bytes
#define MAX_FILENAME_LEN 256

bool FileExist(char *fileName) {
    struct stat buffer;
    return (stat(fileName, &buffer) == 0);
}

bool SplitFile(char *fileName) {
    FILE *inputFile = fopen(fileName, "rb");
    if (!inputFile) {
        printf("fopen failed for '%s' - [%d] %s\n", fileName, errno, strerror(errno));
        return false;
    }

    // Get file size
    if (fseek(inputFile, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking in file '%s' - [%d] %s\n", fileName, errno, strerror(errno));
        fclose(inputFile);
        return false;
    }

    long file_size = ftell(inputFile);
    rewind(inputFile);

    // Allocate 10-byte buffer
    char *buffer = (char *)malloc(TENB * sizeof(char));
    if (!buffer) {
        printf("Buffer memory allocation failed.\n");
        fclose(inputFile);
        return false;
    }

    // Prepare base filename
    char output_filename[MAX_FILENAME_LEN];
    int filename_len = strlen(fileName);
    int base_len = filename_len > 4 ? filename_len - 4 : 0;
    char base_name[MAX_FILENAME_LEN];
    strncpy(base_name, fileName, base_len);
    base_name[base_len] = '\0';

    int chunk_number = 0;
    long bytes_remaining = file_size;

    while (bytes_remaining > 0) {
        size_t current_chunk = (bytes_remaining >= TENB) ? TENB : bytes_remaining;

        size_t read = fread(buffer, sizeof(char), current_chunk, inputFile);
        if (read != current_chunk) {
            printf("Error: Failed to read from input file\n");
            free(buffer);
            fclose(inputFile);
            return false;
        }

        sprintf(output_filename, "%s_part%03d.txt", base_name, chunk_number++);
        FILE *output_file = fopen(output_filename, "wb");
        if (!output_file) {
            printf("fopen failed for '%s' - [%d] %s\n", output_filename, errno, strerror(errno));
            free(buffer);
            fclose(inputFile);
            return false;
        }

        size_t written = fwrite(buffer, sizeof(char), current_chunk, output_file);
        if (written != current_chunk) {
            printf("fwrite failed - '%s' - [%d] %s\n", output_filename, errno, strerror(errno));
            fclose(output_file);
            free(buffer);
            fclose(inputFile);
            return false;
        }

        fclose(output_file);
        printf("Created %s (%ld bytes)\n", output_filename, current_chunk);
        bytes_remaining -= current_chunk;
    }

    fclose(inputFile);
    free(buffer);
    printf("Done. Created %d chunks.\n", chunk_number);
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <Filename>\n", argv[0]);
        return 0;
    }

    char *filename = argv[1];

    if (!SplitFile(filename)) {
        printf("Error in splitting file.\n");
        return 1;
    }

    return 0;
}
