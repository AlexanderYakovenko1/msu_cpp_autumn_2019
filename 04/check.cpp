#include <iostream>
#include <sstream>
#include <cassert>

#include "bigint.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " test_num" << std::endl;
        return 1;
    }

    BigInt a(10);
    BigInt aa(10);
    BigInt max_int(2147483647);
    BigInt min_int(-2147483648);

    switch (std::stoi(argv[1]))
    {
        case 1:
        {
            assert(a == a);
            assert(a == aa);
            break;
        }
        case 2:
        {
            assert((a + 1) > a);
            assert(a - 1 < a);
            break;
        }
        case 3:
        {
            assert(max_int + max_int == -min_int + -min_int - 2);
            break;
        }
        case 4:
        {
            std::cout << max_int + min_int << std::endl;
            break;
        }
        case 5:
        {
            assert(a <= max_int);
            break;
        }
        case 6:
        {
            assert(aa + a == 20);
            break;
        }
        case 7:
        {
            assert(-a == -aa);
            break;
        }
        case 8:
        {
            assert(!(a != a));
            break;
        }
        case 9:
        {
            std::cout << a - a << std::endl;
            break;
        }
        case 10:
        {
            std::cout << max_int - max_int << std::endl;
            break;
        }
        case 11:
        {
            std::cout << min_int - min_int << std::endl;
            break;
        }
        case 12:
        {
            std::cout << min_int - min_int - min_int << std::endl;
            break;
        }
        case 13:
        {
            std::cout << max_int + max_int + max_int << std::endl;
            break;
        }
    }
    return 0;
}