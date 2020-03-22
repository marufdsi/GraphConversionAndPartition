//
// Created by Hossain, Md Maruf on 3/21/20.
//

#ifndef GRAPHCONVERSIONANDPARTITION_GRAPHREADER_H
#define GRAPHCONVERSIONANDPARTITION_GRAPHREADER_H

#include <iostream>
#include "Util.h"
class GraphReader {
public:
    idx_t readMatrix(const std::string filename, idx_t *n, idx_t *nnz, idx_t **i_idx, idx_t **j_idx, ValueType **values);
};


#endif //GRAPHCONVERSIONANDPARTITION_GRAPHREADER_H
