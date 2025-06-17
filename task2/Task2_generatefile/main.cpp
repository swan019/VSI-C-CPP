#include "filegenerate.hpp"
#include <cstdio>     
#include <cstdlib>    

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_file_name>\n", argv[0]);
        return 1;
    }

    string outputFile = argv[1];
    bool isFileGenerate = generateFile(outputFile);
    if (isFileGenerate == false) {
        fprintf(stderr, "Error: File generation failed for '%s'.\n", outputFile.c_str());
        return 1;
    }

    fprintf(stdout, "Info: File '%s' generated successfully.\n", outputFile.c_str());
    return 0;
}
