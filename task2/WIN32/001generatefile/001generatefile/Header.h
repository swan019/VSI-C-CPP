#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define ONE_KB 1024
#define ONE_MB (ONE_KB * ONE_KB)
#define TEN_MB (10 * ONE_MB)
#define TEN 10
#define MAX_LEN 256
#define FIRST_CHAR 'a'
#define BASE_DIR "C:\\Users\\SWAPNIL\\Desktop\\ANT\\VSI\\Tasks\\task2\\WIN32\\001generatefile\\x64\\Debug"

bool FileGenerate(char* fileName);