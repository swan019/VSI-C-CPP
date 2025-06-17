#include "splitfilell.hpp"
#include <cstdio>     
#include <cstdlib>    

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_file_name>\n", argv[0]);
        return 1;
    }

    string inputFile = argv[1];

    bool isFileSplit = splitFile(inputFile);
    if (isFileSplit == false) {
        fprintf(stderr, "Error: File generation failed for '%s'.\n", inputFile.c_str());
        return 1;
    }

    fprintf(stdout, "Info: File '%s' generated successfully.\n", inputFile.c_str());
    return 0;
}
