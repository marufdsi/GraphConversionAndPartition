//
// Created by Hossain, Md Maruf on 3/21/20.
//

#ifndef GRAPHCONVERSIONANDPARTITION_UTIL_H
#define GRAPHCONVERSIONANDPARTITION_UTIL_H

#include <vector>

typedef float ValueType;
typedef int idx_t;


class Parse{
public:
    std::vector<std::string> split(std::string str, char delim=' ');
};


#endif //GRAPHCONVERSIONANDPARTITION_UTIL_H
