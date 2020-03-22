//
// Created by Hossain, Md Maruf on 3/21/20.
//

#ifndef GRAPHCONVERSIONANDPARTITION_RANDOMPARTITION_H
#define GRAPHCONVERSIONANDPARTITION_RANDOMPARTITION_H

#include <iostream>

#include "Util.h"

class RandomPartition {
public:
    idx_t partition(idx_t *i_idx, idx_t *j_idx, ValueType *values, idx_t n, idx_t nnz, idx_t nparts, std::string outdir);
    void swap_position (idx_t *from, idx_t *to);
};


#endif //GRAPHCONVERSIONANDPARTITION_RANDOMPARTITION_H
