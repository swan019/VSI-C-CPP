#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHUNK_SIZE (10 * 1024 * 1024) // 10MB
#define MB (1024 * 1024)

bool split_file(char *input_filename) {
    // Open input file
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        printf("Error: Unable to open input file '%s'\n", input_filename);
        return false;
    }

    // Get file size
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Allocate buffer once
    char *buffer = (char *)malloc(CHUNK_SIZE);
    if (!buffer) {
        printf("Error: Memory allocation failed\n");
        fclose(input_file);
        return false;
    }

    int chunk_number = 0;
    long bytes_remaining = file_size;

    while (bytes_remaining > 0) {
        // Determine current chunk size
        size_t current_chunk = bytes_remaining > CHUNK_SIZE ? CHUNK_SIZE : bytes_remaining;

        // Read chunk
        if (fread(buffer, 1, current_chunk, input_file) != current_chunk) {
            printf("Error: Failed to read from input file\n");
            break;
        }

        // Create output filename
        char output_filename[256];
        snprintf(output_filename, sizeof(output_filename), "%s.part%03d", input_filename, chunk_number++);

        // Write chunk
        FILE *output_file = fopen(output_filename, "wb");
        if (!output_file) {
            printf("Error: Could not create output file '%s'\n", output_filename);
            break;
        }

        if (fwrite(buffer, 1, current_chunk, output_file) != current_chunk) {
            printf("Error: Failed to write to output file '%s'\n", output_filename);
            fclose(output_file);
            break;
        }

        fclose(output_file);
        printf("Created %s (%.2f MB)\n", output_filename, (double)current_chunk / MB);
        bytes_remaining -= current_chunk;
    }

    free(buffer);
    fclose(input_file);
    printf("Done. Created %d chunks.\n", chunk_number);
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    bool isFilesSplit = split_file(argv[1]);
    if (!isFilesSplit) {
        printf("File splitting failed.\n");
        return 1;
    }

    return 0;
}
