#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

template <class T>
std::string VargsToString(const T &t) {
    std::stringstream str;
    str << t;
    return str.str();
}

template <class ...Args>
std::string format(const std::string &s, Args&&... vargs) {
    std::vector<std::string> format_args { VargsToString(std::forward<Args>(vargs))... };

    std::stringstream output;
    
    auto cur = s.begin();
    auto first = s.begin();
    auto second = s.begin();
    while (cur < s.end()) {
        first = std::find(cur, s.end(), '{');
        second = std::find(cur, s.end(), '}');
        
        if (second < first) {
            throw std::runtime_error("Found \'}\' but \'{\' is missing");
        }

        for (; cur < first; ++cur) {
            output << *cur;
        }

        if (first != s.end()) {
            if (second == s.end()) {
                throw std::runtime_error("Found \'{\' but \'}\' is missing");
            }

            int pos;
            try {
                pos = std::stoi(s.substr(std::distance(s.begin(), first) + 1, std::distance(first, second)));
            } catch (std::invalid_argument &e) {
                throw std::runtime_error("Non number characters in bracket sequence");
            }

            if (pos < 0 || pos >= format_args.size()) {
                throw std::runtime_error("Formatting argument out of bounds");
            }

            output << format_args[pos];

            cur = ++second;
        }
    }

    return output.str();
}

#endif