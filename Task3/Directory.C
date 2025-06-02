#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <errno.h>  


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory_path>\n", argv[1]);
        return 1;
    }

    char *path = argv[1];
    printf("Attempting to create directory at: %s\n", path);

    if (_mkdir(path) == 0) {
        printf("Directory created successfully\n");
    } else {
        switch (errno) {
            case EEXIST:
                printf("Directory already exists\n");
                break;
            case ENOENT:
                printf("Parent directory does not exist\n");
                break;
            case EACCES:
                printf("Permission denied\n");
                break;
            default:
                perror("Failed to create directory");
                break;
        }
    }

    return 0;
}