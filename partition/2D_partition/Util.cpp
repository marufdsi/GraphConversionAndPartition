//
// Created by Hossain, Md Maruf on 3/21/20.
//

#include "Util.h"
#include <sstream>
#include <algorithm>
std::vector<std::string> Parse::split(std::string str, char delim) {
    std::stringstream ss(str);
    std::string token;
    std::vector<std::string>tokens;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}
