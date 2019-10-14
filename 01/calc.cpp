#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

void get_char(std::istringstream &stream, int &ch)
{
    while (isspace(ch = stream.get()));
}

int get_num(std::istringstream &stream, int &ch)
{
    if (ch == '-') {
        get_char(stream, ch);
        return -get_num(stream, ch);
    } else if (isdigit(ch)) {
        int tmp;
        stream.unget();
        stream >> tmp;
        get_char(stream, ch);
        return tmp;
    } else {
        throw std::invalid_argument("Unexpected character");
    }
}

int mult_div(std::istringstream &stream, int &ch)
{
    int num = get_num(stream, ch);

    while (true) {
        if (ch == '*') {
            get_char(stream, ch);
            num *= get_num(stream, ch);
        } else if (ch == '/') {
            get_char(stream, ch);
            int div = get_num(stream, ch);
            if (div != 0) {
                num /= div;
            } else {
                throw std::invalid_argument("Division by zero");
            }
        } else {
            return num;
        }
    }
}

int eval(const std::string &str, int &ch)
{
    std::istringstream stream(str);
    get_char(stream, ch);

    int init = mult_div(stream, ch);

    while (ch != EOF) {
        if (ch == '+') {
            get_char(stream, ch);
            init += mult_div(stream, ch);
        } else if (ch == '-') {
            get_char(stream, ch);
            init -= mult_div(stream, ch);
        } else {
            return init;
        }
    }
    return init;
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "\"expression\"" << std::endl;
        return 1;
    }

    int ch = 0;
    int res = 0;
    try {
        res = eval(argv[1], ch);
        if (ch != EOF) {
            throw std::invalid_argument("Unexpected character");
        }
    } catch (std::exception &exc) {
        std::cerr << exc.what() << std::endl;
        return 1;
    }
    std::cout << res << std::endl;

    return 0;
}