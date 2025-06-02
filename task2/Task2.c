#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 10 * 1024 * 1024 

void split_file(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    // Get file size
    fseek(input_file, 0, SEEK_END);
    long file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    int chunk_number = 0;
    long bytes_remaining = file_size;
    char output_filename[256];

    while (bytes_remaining > 0) {
        // Create output filename
        snprintf(output_filename, sizeof(output_filename), "%s.part%d", input_filename, chunk_number);

        // Determine chunk size (last chunk may be smaller)
        long current_chunk_size = (bytes_remaining > CHUNK_SIZE) ? CHUNK_SIZE : bytes_remaining;

        // Open output file
        FILE *output_file = fopen(output_filename, "wb");
        if (!output_file) {
            perror("Error opening output file");
            fclose(input_file);
            exit(EXIT_FAILURE);
        }

        // Buffer for reading/writing
        char *buffer = (char*)malloc(current_chunk_size);
        if (!buffer) {
            perror("Memory allocation failed");
            fclose(input_file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        // Read and write chunk
        size_t bytes_read = fread(buffer, 1, current_chunk_size, input_file);
        if (bytes_read != current_chunk_size) {
            perror("Error reading input file");
            free(buffer);
            fclose(input_file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        size_t bytes_written = fwrite(buffer, 1, current_chunk_size, output_file);
        if (bytes_written != current_chunk_size) {
            perror("Error writing to output file");
            free(buffer);
            fclose(input_file);
            fclose(output_file);
            exit(EXIT_FAILURE);
        }

        // Clean up
        free(buffer);
        fclose(output_file);

        // Update counters
        bytes_remaining -= current_chunk_size;
        chunk_number++;

        printf("Created chunk: %s (%ld bytes)\n", output_filename, current_chunk_size);
    }

    fclose(input_file);
    printf("File split completed. Total chunks: %d\n", chunk_number);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    split_file(argv[1]);
    return EXIT_SUCCESS;
}
