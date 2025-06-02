#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#define FILE_SIZE    (10 * 1024 * 1024)  // 10MB
#define LINE_LENGTH  80


// Returns a random letter (A-Z, a-z)
char getRandomChar() {
    const std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int index = std::rand() % letters.size();
    return letters[index];
}

// Returns a random digit (0–9)
char getRandomDigit() {
    return '0' + (std::rand() % 10);
}

// Function to generate the file
void generateFile(std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "Failed to open file.\n";
        return;
    }

    int count = 0;
    while (count < FILE_SIZE) {
        char ch;
        if (count % LINE_LENGTH == LINE_LENGTH - 1) {
            ch = '\n';
        } else {
            // Alternate lines: even = letters, odd = digits
            if ((count / LINE_LENGTH) % 2 == 0)
                ch = getRandomChar();
            else
                ch = getRandomDigit();
        }

        file.put(ch);
        count++;

        if (count % (1024 * 1024) == 0)
            std::cout << "Written " << (count / (1024 * 1024)) << "MB...\n";
    }

    std::cout << "Finished writing 10MB to file: " << filename << "\n";
}

int main() {
    std::string filename;
    std::cout << "Enter filename to generate random data: ";
    std::cin >> filename;

    generateFile(filename);

    return 0;
}
