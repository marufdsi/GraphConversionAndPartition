//
// Created by Hossain, Md Maruf on 3/21/20.
//
#include <time.h>
#include <math.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "RandomPartition.h"

// A utility function to swap to integers
void RandomPartition::swap_position (int *from, int *to)
{
    int temp = *from;
    *from = *to;
    *to = temp;
}

int RandomPartition::partition(int *i_idx, int *j_idx, ValueType *values, int n, int nnz, int nparts, std::string outdir) {
    idx_t MAXLINE = 1000;
    idx_t u, i, v;
    idx_t *random_vertex = (idx_t *)malloc(n* sizeof(idx_t));
    idx_t row = sqrt(nparts);
    idx_t col = row;
    for(u=0; u<n; ++u){
        random_vertex[u] = u;
    }
    srand ( time(NULL) );
    for (i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        // Swap arr[i] with the element at random index
        swap_position(&random_vertex[i], &random_vertex[j]);
    }
    idx_t new_id = 0;
    idx_t * new_ids;
    new_ids = (idx_t *)malloc(n * sizeof(idx_t));
    int num_row = ceil(((double)n)/row);
    idx_t _part = 0, nVartex = 0, start = 0;
    for (int i = 0; i < n; ++i) {
        new_ids[random_vertex[i]] = new_id++;
    }
    idx_t *nEdges_part = (idx_t *)malloc(nparts * sizeof(idx_t));
    for (int k = 0; k < nparts; ++k) {
        nEdges_part[k] = 0;
    }
    for (int i = 0; i < n; ++i) {
        nVartex++;
        for (int v = i_idx[random_vertex[i]]; v < i_idx[random_vertex[i] + 1]; v++) {
            idx_t col_part = new_ids[j_idx[v]]/num_row;
            nEdges_part[_part + col_part] += 1;
        }
        idx_t condition = (num_row * (_part/col + 1)) > n ? n : (num_row * (_part/col + 1));
        if((i+1) >= condition) {
//            int startIdx = _part*num_row;
//            printf("Part=%d, row=%d, col=%d\n", _part, row, col);
            for (int cl = 0; cl < col; ++cl) {
//                FILE *newMat;
                std::ofstream outfile;
                char mat_filename[MAXLINE];
                sprintf(mat_filename, "%s_random_%d_%d", outdir.c_str(), nparts, (_part+cl));
                outfile.open(mat_filename, std::ios::out | std::ios::trunc );
                /*if (!(newMat = fopen(std::strcat(mat_filename, ".mtx"), "w"))) {
                    std::cerr << "fopen: failed to open file '" << mat_filename << "'" << std::endl;
                    return 1;
                }*/
//                fprintf(newMat, "%%%MatrixMarket matrix coordinate real general\n");
//                fprintf(newMat, "%d %d %d\n", nVartex, n, nEdges_part[_part + cl]);
                outfile << "%%%MatrixMarket matrix coordinate real general" << std::endl;
                outfile << nVartex << " "<< n << " " << nEdges_part[_part + cl] << std::endl;

                for (int itr = start; itr <= i; ++itr) {
                    u = random_vertex[itr];
                    if(new_ids[u] > i){
                        std::cout << u << " mapped wrong as " << new_ids[u] <<std::endl;
                    }
                    for (v = i_idx[u]; v < i_idx[u + 1]; v++) {
                        if(new_ids[j_idx[v]]/num_row == cl) {
//                            fprintf(newMat, "%d %d %lf\n", (new_ids[u] + 1), (new_ids[j_idx[v]] + 1), (double) values[v]);
                            outfile << (new_ids[u] + 1) << " "<< (new_ids[j_idx[v]] + 1) << " " << values[v] << std::endl;
                        }
                    }
                }
//                printf("Done=%d\n", cl);

//                 close file
                outfile.close();
                /*if (fclose(newMat) != 0) {
                    std::cerr << "fopen: failed to close file '"<<  mat_filename << "'" <<std::endl;
                    return 1;
                }*/
            }
            nVartex = 0;
            start = i + 1;
            _part += col;
        }
    }

    free(new_ids);
    free(random_vertex);
    free(nEdges_part);

    return 0;
}