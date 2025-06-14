#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define ONEKB (1024)
#define ONEMB (ONEKB * 1024)
#define TENMB (ONEMB * 10) // 10MB
#define MAX_FILENAME_LEN 256

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
        fprintf(stderr, "Error seeking in file '%s' - [%d] %s\n", 
                inputFile, errno, strerror(errno));
        fclose(inputFile);
        inputFile = NULL;
        return false;
    }
    long file_size = ftell(inputFile);
    rewind(inputFile);
//--------------------------------------------------------------------------------------------------------------

//------------------------------ Allocate buffer for read from main file----------------------------------------- 
    char *buffer = (char *)malloc(TENMB*sizeof(char));
    if (!buffer)
    {
        printf("Buffer memory allocation failed.\n");
        fclose(inputFile);
        return false;
    }
//----------------------------------------------------------------------------------------------------------------


//------------------------------------------ Create output filename-----------------------------------------------
    char output_filename[MAX_FILENAME_LEN];
    char *dot = strrchr(fileName, '.');  

    int base_len;
    if (dot != NULL) {
        base_len = dot - fileName;  
    } else {
        base_len = strlen(fileName); 
    }
    char base_name[MAX_FILENAME_LEN];
    strncpy(base_name, fileName, base_len);
    base_name[base_len] = '\0';

    const char *extension;
    if (dot != NULL) {
        extension = dot;  
    } else {
        extension = "";   
    }

/*

    Example 1: file.txt
    fileName = "file.txt"
    dot points to ".txt"
    base_len = 4 (index of dot)
    base_name = "file"
    ext = ".txt"

*/

//----------------------------------------------------------------------------------------------------------------


    int chunk_number = 0;
    long bytes_remaining = file_size;

    size_t current_chunk = 0;  
    size_t read = 0;
    FILE *output_file = NULL;
    size_t written = 0;
    
    while (bytes_remaining > 0)
    {    
        if (bytes_remaining >= TENMB) {
            current_chunk = TENMB;  //Take full chunk if enough data remains
        }
        else {
            current_chunk = bytes_remaining; //Take remaining data for final partial chunk 
        }

//--------------------------------------- read file data into buffer----------------------------------------------------
        read = fread(buffer, sizeof(char), current_chunk, inputFile);
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
        sprintf(output_filename, "%s_part%03d%s", base_name, chunk_number++, extension); // filename_part001.txt

        output_file = fopen(output_filename, "wb");
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

        written = fwrite(buffer, sizeof(char), current_chunk, output_file); // write buffer data to split files
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

        printf("Created %s (%.2f MB)\n", output_filename, (double)current_chunk / ONEMB);
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

    if (!SplitFile(filename))
    {
        printf("Error in splitting file.\n");
        return 1;
    }

    return 0;
}
