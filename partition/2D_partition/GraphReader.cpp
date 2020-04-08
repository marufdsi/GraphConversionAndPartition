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
    idx_t nrows, ncols, nz_elements, isSymmetric=0;
    char banner[MM_MAX_TOKEN_LENGTH];
    char mtx[MM_MAX_TOKEN_LENGTH];
    char crd[MM_MAX_TOKEN_LENGTH];
    char data_type[MM_MAX_TOKEN_LENGTH];
    char storage_scheme[MM_MAX_TOKEN_LENGTH];
    // Open the file:
    std::ifstream f_head(filename.c_str());
    f_head >> banner >> mtx >> crd >> data_type >> storage_scheme;
    std::cout<< banner << "\t" << mtx << "\t" << crd << "\t" << data_type << "\t" << storage_scheme << std::endl;
    f_head.close();
    if (strcmp(storage_scheme, MM_SYMM_STR) == 0)
        isSymmetric = 1;
    if (std::strcmp(storage_scheme, MM_SYMM_STR) != 0 && std::strcmp(storage_scheme, MM_GENERAL_STR) != 0){
        std::cout<<"Matrix is not symmetric or general!" <<std::endl;
        return MM_UNSUPPORTED_TYPE;
    }
    std::ifstream fin(filename.c_str());
// Ignore headers and comments:
    while (fin.peek() == '%') fin.ignore(2048, '\n');

// Read defining parameters:
    fin >> nrows >> ncols >> nz_elements;
    idx_t *csrRowPtrA_counter = (idx_t *) malloc((nrows + 1) * sizeof(idx_t));
    for (i = 0; i < nrows + 1; ++i) {
        csrRowPtrA_counter[i] = 0;
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
        if (std::strcmp(data_type, MM_REAL_STR) == 0) {
            fin >> idxi >> idxj >> fval;
        } else if (std::strcmp(data_type, MM_PATTERN_STR) == 0){
            fin >> idxi >> idxj;
            fval = 1.0;
        } else if (std::strcmp(data_type, MM_INT_STR) == 0){
            fin >> idxi >> idxj >> ival;
            fval = ival;
        } else {
            std::cout<<"Unsupported data type!" <<std::endl;
            return MM_UNSUPPORTED_TYPE;
        }

        idxi--;
        idxj--;
        csrRowPtrA_counter[idxi]++;
        i_idx[i] = idxi;
        j_idx[i] = idxj;
        values[i] = fval;
    }
    fin.close();
    if (isSymmetric) {
        for (i = 0; i < nz; i++) {
            if (i_idx[i] != j_idx[i])
                csrRowPtrA_counter[j_idx[i]]++;
        }
    }
    idx_t old_val, new_val;
    old_val = csrRowPtrA_counter[0];
    csrRowPtrA_counter[0] = 0;
    for (i = 1; i <= nrows; i++) {
        new_val = csrRowPtrA_counter[i];
        csrRowPtrA_counter[i] = old_val + csrRowPtrA_counter[i - 1];
        old_val = new_val;
    }
    /// Initialize CSR row, col and value pointer.
    *n = nrows;
    *nnz = csrRowPtrA_counter[nrows];
    std::cout<<"rows: " << nrows << " nnz elements: " << *nnz << std::endl;
    (*row_ptr) = (idx_t *) malloc((nrows + 1) * sizeof(idx_t));
    memcpy((*row_ptr), csrRowPtrA_counter, (nrows + 1) * sizeof(idx_t));
    memset(csrRowPtrA_counter, 0, (nrows + 1) * sizeof(idx_t));
    (*col_ptr) = (idx_t *) malloc((*nnz) * sizeof(idx_t));
    (*val_ptr) = (ValueType *) malloc((*nnz) * sizeof(ValueType));

    if (isSymmetric) {
        for (i = 0; i < nz_elements; i++) {
            if (i_idx[i] != j_idx[i]) {
                int offset = (*row_ptr)[i_idx[i]] + csrRowPtrA_counter[i_idx[i]];
                (*col_ptr)[offset] = j_idx[i];
                (*val_ptr)[offset] = values[i];
                csrRowPtrA_counter[i_idx[i]]++;

                offset = (*row_ptr)[j_idx[i]] + csrRowPtrA_counter[j_idx[i]];
                (*col_ptr)[offset] = j_idx[i];
                (*val_ptr)[offset] = values[i];
                csrRowPtrA_counter[j_idx[i]]++;
            } else {
                int offset = (*row_ptr)[csrRowIdxA_tmp[i]] + csrRowPtrA_counter[csrRowIdxA_tmp[i]];
                (*col_ptr)[offset] = j_idx[i];
                (*val_ptr)[offset] = values[i];
                csrRowPtrA_counter[i_idx[i]]++;
            }
        }
    } else {
        for (i = 0; i < nz_elements; i++) {
            int offset = (*row_ptr)[i_idx[i]] + csrRowPtrA_counter[i_idx[i]];
            (*col_ptr)[offset] = j_idx[i];
            (*val_ptr)[offset] = values[i];
            csrRowPtrA_counter[i_idx[i]]++;
        }
    }
    free(i_idx);
    free(j_idx);
    free(values);
    free(csrRowPtrA_counter);
    return 0;
}