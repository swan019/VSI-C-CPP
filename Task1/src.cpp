#include <iostream>
#include <fstream>
#include <random>
#include <string>

int main() {
    const std::string filename = "random_data.txt";
    const int file_size = 100 * 1024 * 1024; // 100MB
    const int line_length = 80;

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int charset_size = chars.size();

    int count = 0;
    while (count < file_size) {
        char ch = chars[gen() % charset_size];  // Avoid uniform_int_distribution
        if (count % line_length == line_length - 1)
            ch = '\n';

        file.put(ch);
        count++;

        if (count % (1024 * 1024) == 0)
            std::cout << "Written " << (count / (1024 * 1024)) << "MB...\n";
    }

    std::cout << "Finished writing " << file_size << " bytes to " << filename << "\n";
    return 0;
}
