//
// Created by Hossain, Md Maruf on 3/21/20.
//

#include "GraphReader.h"
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <time.h>

idx_t GraphReader::readMatrix(const std::string filename, idx_t *n, idx_t *nnz, idx_t **row_ptr, idx_t **col_ptr, ValueType **val_ptr){
    idx_t nrows, ncols, nz_elements;
    // Open the file:
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
//        fin >> i_idx[i] >> j_idx[i] >> values[i];
        fin >> i_idx[i] >> j_idx[i];
        values[i] = rand()%((nrows/10000) + 1 );
        /*if(i_idx[i] == 0 || j_idx[i] == 0){
            fin >> i_idx[i] >> j_idx[i];
            values[i] = rand()%((nrows/10000) + 1 );
        }*/
//        std::cout<<i_idx[i] << " , " << j_idx[i] << " , " << values[i] << std::endl;
        i_idx[i]--;
        j_idx[i]--;
    }

    for (int i = 0; i < nz_elements; i++) {
        if (i_idx[i] >= nrows || i_idx[i] < 0) {
            printf("Index out of bound for row=%d\n", i_idx[i]);
            return 1;
        }
        ((*row_ptr)[i_idx[i]])++;
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