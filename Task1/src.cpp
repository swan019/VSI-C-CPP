#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdio> 
#include <sys/stat.h>

#define FILE_SIZE (10 * 1024 * 1024) // 10MB
#define LINE_LENGTH 80

using namespace std;

char getRandomChar()
{
    const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    return letters[rand() % letters.size()];
}

char getRandomDigit()
{
    return '0' + (rand() % 10);
}


//to check file is already exist
bool fileExists(string &filename)
{
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}


void generateFile(string &filename, string &mode)
{
    fstream file(filename, ios::out | ios::binary);
    if (!file)
    {
        printf("Failed to open file.\n");
        return;
    }

    int count = 0;
    int lastPrintedMB = 0;  // To track how many MBs have been printed

    while (count < FILE_SIZE)
    {
        for (int i = 0; i < LINE_LENGTH - 1; ++i)
        {
            char ch;
            if (mode == "digit"){
                ch = getRandomDigit();
            }
            else{
                ch = getRandomChar();
            }

            file.put(ch);
            ++count;
        }
        file.put('\n');
        ++count;

        int currentMB = count / (1024 * 1024);
        if (currentMB > lastPrintedMB)
        {
            printf("Written %dMB...\n", currentMB);
            lastPrintedMB = currentMB;
        }
    }

    file.close();
    printf("Finished writing 10MB to file: %s\n", filename.c_str());
}


void fileCreate()
{
    string filename, mode;
    printf("Enter filename: ");
    cin >> filename;

    if (fileExists(filename))
    {
        printf("File '%s' already exists! Overwrite? (y/n): ", filename.c_str());
        char choice;
        cin >> choice;
        if (choice != 'y' && choice != 'Y')
        {
            printf("Aborting file write.\n");
            return;
        }
    }

    printf("Enter mode (digit / char): ");
    cin >> mode;

    if (mode != "digit" && mode != "char")
    {
        printf("Invalid mode. Please enter 'digit' or 'char'.\n");
        return;
    }

    generateFile(filename, mode);
}

int main()
{
    fileCreate();

    return 0;
}
