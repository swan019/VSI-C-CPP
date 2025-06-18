#ifndef FILE_GENERATE_HPP
#define FILE_GENERATE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cerrno>
using namespace std;

#define ONE_KB 1024
#define ONE_MB (1024 * ONE_KB)
#define TEN_MB 10
#define KB_BUFFER_SIZE ONE_KB
#define NODE_SIZE   (1024 * KB_BUFFER_SIZE) // 1MB

bool generateFile(string& fileName);

#endif
