#include <iostream>
#include <sstream>

#include "format.h"

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "test_num" << std::endl;
        return 1;
    }

    switch (std::stoi(argv[1]))
    {
        case 1: 
        {
            std::cout << format("") << std::endl;
            break;
        }
        case 2: {
            std::cout << format("{1} + {1} = {0}", 2, 1) << std::endl;
            break;
        }
        case 3: {
            std::cout << format("{0}+{1}={2}", 1, 9, "B") << std::endl;
            break;
        }
        case 4: {
            try {
                std::cout << format("{3}", "some", "filler", "here") << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            break;
        }
        case 5: {
            try {
                std::cout << format("{{{}}}") << std::endl;
            } catch (std::runtime_error& e) {
                std::cout << e.what() << std::endl;
            }
            break;
        }
        case 6: {
            try {
                std::cout << format("}{1}", 3, 5, 6) << std::endl;
            } catch (std::runtime_error& e) {
                    std::cout << e.what() << std::endl;
            }
            break;
        }
        case 7: {
            std::cout << format("{0}*{1}*{2}*{3}", 0, "one", 2, "ten") << std::endl;
            break;
        }
        case 8: {
            try {
                std::cout << format("Sample Text{") << std::endl;
            } catch (std::runtime_error& e) {
                    std::cout << e.what() << std::endl;
            }
        }
    }
    return 0;
}