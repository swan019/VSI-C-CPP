#include "filegenerate.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <cerrno>

using namespace std;

bool generateFile(string& fileName) {
    ofstream p_outFile(fileName, ios::binary);
    if (!p_outFile) {
        fprintf(stderr, "Error: Failed to open file '%s' for writing - [%d] %s\n",
            fileName.c_str(), errno, strerror(errno));
        return false;
    }

    char kbBuffer[KB_BUFFER_SIZE];

    for (int i = 0; i < TEN_MB; i++) {
        char ch = 'a' + i;
        memset(kbBuffer, ch, KB_BUFFER_SIZE);

        for (int j = 0; j < NODE_SIZE / KB_BUFFER_SIZE; ++j) {
            p_outFile.write(kbBuffer, KB_BUFFER_SIZE);
            if (!p_outFile) {
                fprintf(stderr, "Error: Failed to write at block %dMB - [%d] %s\n",
                    i + 1, errno, strerror(errno));
                p_outFile.close();
                return false;
            }
        }
    }

    p_outFile.close();
    if (!p_outFile) {
        fprintf(stderr, "Warning: File close may have failed for '%s'.\n", fileName.c_str());
        return false;
    }

    fprintf(stdout, "Successfully generated %dMB file '%s' using 1KB buffer.\n",
        TEN_MB, fileName.c_str());
    return true;
}