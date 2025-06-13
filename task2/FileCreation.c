#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <errno.h>

#define CHUNK_SIZE (10 * 1024 * 1024)
#define TOTAL_CHUNKS 10

bool FileGenerate(char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("fopen failed for '%s' - [%d] %s\n", filename, errno, strerror(errno));
        return false;
    }

    char *buffer = (char *)malloc(CHUNK_SIZE);

    if (!buffer)
    {
        fprintf(stderr, "Buffer memory allocation failed.\n");
        fclose(fp);
        return false;
    }

    memset(buffer, '0', sizeof(char) * CHUNK_SIZE);
    char fill_char;
    size_t bytes_written;

    for (int i = 0; i < TOTAL_CHUNKS; i++)
    {
        fill_char = '0' + i;
        memset(buffer, fill_char, sizeof(char) * CHUNK_SIZE);
        
        bytes_written = fwrite(buffer, 1, CHUNK_SIZE, fp);

        if (bytes_written != CHUNK_SIZE)
        {
            fprintf(stderr, "fwrite failed - '%s' - [%d] %s\n", filename, errno, strerror(errno));
            free(buffer);
            fclose(fp);
            return false;
        }

        printf("Written 10MB of '%c'\n", fill_char);
    }

    memset(buffer, '0', sizeof(char) * CHUNK_SIZE);
    free(buffer);
    buffer = NULL;
    fclose(fp);
    fp = NULL;
    printf("File '%s' created successfully.\n", filename);
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage : %s <Filename> \n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    bool isFileGenerate = FileGenerate(filename);
    if (!isFileGenerate)
    {
        printf("Error in generate file.\n");
        return 1;
    }
    return 0;
}