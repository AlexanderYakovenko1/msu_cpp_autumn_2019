#include <cstdlib>
#include <iostream>
#include <sstream>

int ch = 0;

void get_char(std::istringstream &stream) {
    while (isspace(ch = stream.get()));
}

int get_num(std::istringstream &stream) {
    if (ch == '-') {
        get_char(stream);
        return -get_num(stream);
    } else if (isdigit(ch)) {
        int tmp;
        stream.unget();
        stream >> tmp;
        get_char(stream);
        return tmp;
    } else {
        fprintf(stderr, "Unexpected character %c\n", ch);
        exit(1);
    }
}

int mult_div(std::istringstream &stream) {
    int num = get_num(stream);

    while (true) {
        if (ch == '*') {
            get_char(stream);
            num *= get_num(stream);
        } else if (ch == '/') {
            get_char(stream);
            int div = get_num(stream);
            if (div != 0) {
                num /= div;
            } else {
                fprintf(stderr, "Division by zero\n");
                exit(1);
            }
        } else {
            return num;
        }
    }
}

int eval(const std::string &str) {
    std::istringstream stream(str);
    get_char(stream);

    int init = mult_div(stream);

    while (ch != EOF) {
        if (ch == '+') {
            get_char(stream);
            init += mult_div(stream);
        } else if (ch == '-') {
            get_char(stream);
            init -= mult_div(stream);
        } else {
            return init;
        }
    }
    return init;
}


int main(int argc, char const *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s \"expression\"\n", argv[0]);
        return 1;
    }

    int res = eval(argv[1]);
    if (ch != EOF) {
        fprintf(stderr, "Unexpected character %c\n", ch);
        return 1;
    }
    printf("%d\n", res);

    return 0;
}