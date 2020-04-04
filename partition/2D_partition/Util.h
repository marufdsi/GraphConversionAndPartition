//
// Created by Hossain, Md Maruf on 3/21/20.
//

#ifndef GRAPHCONVERSIONANDPARTITION_UTIL_H
#define GRAPHCONVERSIONANDPARTITION_UTIL_H

#include <vector>
#include <iostream>
typedef float ValueType;
typedef int idx_t;

#define MM_MAX_TOKEN_LENGTH 64
#define MM_UNSUPPORTED_TYPE -1
#define MM_COMPLEX_STR	"complex"
#define MM_REAL_STR		"real"
#define MM_INT_STR		"integer"
#define MM_PATTERN_STR  "pattern"
#define MM_SYMM_STR		"symmetric"
#define MM_GENERAL_STR  "general"


class Parse{
public:
    std::vector<std::string> split(std::string str, char delim=' ');
};


#endif //GRAPHCONVERSIONANDPARTITION_UTIL_H
