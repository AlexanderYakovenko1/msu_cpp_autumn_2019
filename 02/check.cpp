#include <iostream>
#include <sstream>

#include "alloc.h"

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "alloc size" << " \"operations\"\n"
                  << "Valid operations: %d - alloc length, r - reset" << std::endl;
        return 1;
    }

    LinearAllocator lalloc(std::stoi(argv[1]));
    std::istringstream in(argv[2]);
    int ch;
    while (!in.eof()) {
        ch = in.get();
        size_t length;

        if (ch == 'r') {
            lalloc.reset();
            std::cout << "r ";
        } else if (isdigit(ch)) {
            in.unget();
            in >> length;
            char *ptr = lalloc.alloc(length);
            std::cout << (ptr ? "ptr " : "nullptr ");
        }
    }
    std::cout << std::endl;
    return 0;
}