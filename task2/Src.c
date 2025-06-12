#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define CHUNK_SIZE (10 * 1024 * 1024) // 10MB
#define MB (1024 * 1024)
#define MAX_FILENAME_LEN 256

bool FileExist(char *fileName)
{
    struct stat buffer;
    bool isExist = stat(fileName, &buffer) == 0;
    return (isExist);
}

bool SplitFile(char *fileName)
{
    FILE *inputFile = fopen(fileName, "rb"); // file open
    if (!inputFile)
    {
        printf("fopen failed for '%s' - [%d] %s\n", fileName, errno, strerror(errno));
        return false;
    }

// -------------------------------------Get file size --------------------------------------------------------
    int seek = fseek(inputFile, 0, SEEK_END);
    if (seek != 0)
    {
        fprintf(stderr, "Error seeking in file '%s' - [%d] %s\n", inputFile, errno, strerror(errno));
        fclose(inputFile);
        inputFile = NULL;
        return false;
    }
    long file_size = ftell(inputFile);
    rewind(inputFile);
//--------------------------------------------------------------------------------------------------------------

//------------------------------ Allocate buffer for read from main file----------------------------------------- 
    char *buffer = (char *)malloc(CHUNK_SIZE);

    if (!buffer)
    {
        printf("Buffer memory allocation failed.\n");
        fclose(inputFile);
        return false;
    }
//----------------------------------------------------------------------------------------------------------------


//------------------------------------------ Create output filename-----------------------------------------------
    char output_filename[MAX_FILENAME_LEN];
    int filename_len = strlen(fileName);
    int base_len = filename_len > 4 ? filename_len - 4 : 0;
    char base_name[MAX_FILENAME_LEN];
    strncpy(base_name, fileName, base_len);
    base_name[base_len] = '\0'; 

    //filename.txt -> filename
//----------------------------------------------------------------------------------------------------------------


    int chunk_number = 0;
    long bytes_remaining = file_size;
    while (bytes_remaining > 0)
    {
        size_t current_chunk = 0;    
        if (bytes_remaining >= CHUNK_SIZE) {
            current_chunk = CHUNK_SIZE;  //Take full chunk if enough data remains
        }
        else {
            current_chunk = bytes_remaining; //Take remaining data for final partial chunk 
        }

//--------------------------------------- read file data into buffer----------------------------------------------------
        size_t read = fread(buffer, sizeof(char), current_chunk, inputFile);
        if (read != current_chunk)
        {
            printf("Error: Failed to read from input file\n");
            free(buffer);
            fclose(inputFile);
            inputFile = NULL;
            buffer = NULL;
            return false;
        }
//--------------------------------------------------------------------------------------------------------------------------
        sprintf(output_filename, "%s_part%03d.txt", base_name, chunk_number++); // filename_part001.txt

        FILE *output_file = fopen(output_filename, "wb");
        if (!output_file)
        {
            printf("fopen failed for '%s' - [%d] %s\n", output_filename, errno, strerror(errno));
            free(buffer);
            fclose(inputFile);
            inputFile = NULL;
            buffer = NULL;
            return false;
        }

//-------------------------------------------------------------------------------------------------------------------------

        size_t written = fwrite(buffer, sizeof(char), current_chunk, output_file); // write buffer data to split files
        if (written != current_chunk)
        {
            printf("fwrite failed - '%s' - [%d] %s\n", output_filename, errno, strerror(errno));
            fclose(output_file);
            free(buffer);
            fclose(inputFile);
            buffer = NULL;
            inputFile = NULL;
            return false;
        }

        fclose(output_file);
        output_file = NULL;

//-------------------------------------------------------------------------------------------------------------------------

        printf("Created %s (%.2f MB)\n", output_filename, (double)current_chunk / MB);
        bytes_remaining -= current_chunk;
    }

    fclose(inputFile);
    free(buffer);
    inputFile = NULL;
    buffer = NULL;
    printf("Done. Created %d chunks.\n", chunk_number);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <Filename>\n", argv[0]);
        return 0;
    }

    char *filename = argv[1];
    if (!FileExist(filename))  // To cheak file exist
    {
        printf("File not present in current directory.\n");
        return 1;
    }

    if (!SplitFile(filename))
    {
        printf("Error in splitting file.\n");
        return 1;
    }

    return 0;
}
