#include <iostream>
#include <sstream>
#include <cassert>

#include "matrix.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "test_num" << std::endl;
        return 1;
    }

    const size_t rows = 5;
    const size_t cols = 3;
    Matrix m(rows, cols);
    const Matrix m1(rows, cols);

    switch (std::stoi(argv[1]))
    {
        case 1:
        {
            assert(m.getRows() == 5);
            assert(m.getColumns() == 3);
            break;
        }
        case 2:
        {
            m[1][2] = 5;
            break;
        }
        case 3:
        {
            double x = m[4][1];
            break;
        }
        case 4:
        {
            m *= 3;
            break;
        }
        case 5:
        {
            assert(m == m);
            break;
        }
        case 6:
        {
            assert(m1 == m1);
            break;
        }
        case 7:
        {
            m[0][0] = 10;
            assert(m1 != m);
            assert(m != m1);
            break;
        }
        case 8:
        {
            assert(!(m != m));
            break;
        }
        case 9:
        {
            assert(!(m1 != m1));
            break;
        }
        case 10:
        {
            try {
                m[10][0] = 2;
            } catch (std::exception &exc) {
                std::cout << exc.what() << std::endl;
                return 1;
            }
        }
        case 11:
        {
            try {
                m[0][10] = 2;
            } catch (std::exception &exc) {
                std::cout << exc.what() << std::endl;
                return 1;
            }
        }
    }
    return 0;
}