//
// Created by Hossain, Md Maruf on 3/21/20.
//

#include "GraphReader.h"
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <cstring>

idx_t GraphReader::readMatrix(const std::string filename, idx_t *n, idx_t *nnz, idx_t **row_ptr, idx_t **col_ptr, ValueType **val_ptr){
    idx_t nrows, ncols, nz_elements;
//    char banner[MM_MAX_TOKEN_LENGTH];
    std::string banner;
//    char mtx[MM_MAX_TOKEN_LENGTH];
    std::string mtx;
//    char crd[MM_MAX_TOKEN_LENGTH];
    string crd;
//    char data_type[MM_MAX_TOKEN_LENGTH];
    std::string data_type;
//    char storage_scheme[MM_MAX_TOKEN_LENGTH];
    std::string storage_scheme;
    // Open the file:
    std::ifstream f_head(filename.c_str());
    f_head >> banner >> mtx >> crd >> data_type >> storage_scheme;
    std::cout<< banner << "\t" << mtx << "\t" << crd << "\t" << data_type << "\t" << storage_scheme << std::endl;
    f_head.close();
    if (storage_scheme != MM_SYMM_STR && storage_scheme != MM_GENERAL_STR){
        std::cout<<"Matrix is not symmetric or general! " << storage_scheme <<std::endl;
        return MM_UNSUPPORTED_TYPE;
    }
    std::ifstream fin(filename.c_str());
// Ignore headers and comments:
    while (fin.peek() == '%') fin.ignore(2048, '\n');

// Read defining parameters:
    fin >> nrows >> ncols >> nz_elements;
    *n = nrows;
    *nnz = nz_elements;
    std::cout<<"rows: " << nrows << " nnz elements: " << nz_elements << std::endl;
    /// Initialize CSR row, col and value pointer.
    (*row_ptr) = (idx_t *) calloc((nrows + 1), sizeof(idx_t));
    (*col_ptr) = (idx_t *) malloc(nz_elements * sizeof(idx_t));
    (*val_ptr) = (ValueType *) malloc(nz_elements * sizeof(ValueType));
    (*row_ptr)[0] = 0;
    for (int i = 0; i < nrows; i++) {
        if((*row_ptr)[i] != 0){
            std::cout<<"Not zer for " << (*row_ptr)[i] << " index: " << i << std::endl;
        }
    }

    /* reserve memory for vector */
    idx_t *i_idx = (idx_t *) malloc(nz_elements * sizeof(idx_t));
    idx_t *j_idx = (idx_t *) malloc(nz_elements * sizeof(idx_t));
    ValueType *values = (ValueType *) malloc(nz_elements * sizeof(ValueType));
//    srand ( time(NULL) );
    for (int i = 0; i < *nnz; i++) {
        int idxi, idxj;
        ValueType fval;
        int ival;
        if (data_type == MM_REAL_STR) {
            fin >> idxi >> idxj >> fval;
        } else if (data_type == MM_PATTERN_STR){
            fin >> idxi >> idxj;
            fval = 1.0;
        } else if (data_type == MM_INT_STR){
            fin >> idxi >> idxj >> ival;
            fval = ival;
        } else {
            std::cout<<"Unsupported data type!" <<std::endl;
            return MM_UNSUPPORTED_TYPE;
        }

        idxi--;
        idxj--;
        ((*row_ptr)[idxi])++;
        i_idx[i] = idxi;
        j_idx[i] = idxj;
        values[i] = fval;
    }
    for (int i = 0, cumsum = 0; i < nrows; i++) {
        int temp = (*row_ptr)[i];
        (*row_ptr)[i] = cumsum;
        cumsum += temp;
    }
    (*row_ptr)[nrows] = nz_elements;

    for (int n = 0; n < nz_elements; n++) {
        int row = i_idx[n];
        if (row < 0 || row >= nrows) {
            printf("out of bound for row=%d\n", row);
        }
        int dest = (*row_ptr)[row];
        (*col_ptr)[dest] = j_idx[n];
        (*val_ptr)[dest] = values[n];

        (*row_ptr)[row]++;
    }

    for (int i = 0, last = 0; i <= nrows; i++) {
        int temp = (*row_ptr)[i];
        (*row_ptr)[i] = last;
        last = temp;
    }

    fin.close();
    free(i_idx);
    free(j_idx);
    free(values);
    return 0;
}