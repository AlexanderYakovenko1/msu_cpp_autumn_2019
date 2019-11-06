#include <iostream>
#include <sstream>
#include <cassert>

#include "bigint.h"

int main(int argc, char const *argv[])
{
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << "test_num" << std::endl;
    //     return 1;
    // }

    BigInt a = -2000000000;
    BigInt b = -2000000000;

    std::cout << a - b << std::endl;

    return 0;
}