//
// Created by Hossain, Md Maruf on 3/21/20.
//

#include <iostream>
#include <vector>

#include "Util.h"
#include "GraphReader.h"
#include "RandomPartition.h"

int main(int argc, char *argv[]){
    std::string filename, outdir;
    idx_t n, nnz, *i_idx, *j_idx, nparts;
    ValueType *values;
    if(argc>3) {
        filename = argv[1];
        nparts = std::atoi(argv[2]);
        outdir = argv[3];
    } else {
        std::cout<<"call: " << argv[0] << " filename nparts outdir" << std::endl;
        return 0;
    }
    GraphReader reader;
    if(reader.readMatrix(filename, &n, &nnz, &i_idx, &j_idx, &values) != 0){
        std::cout<<"Read Failed"<<std::endl;
        return 0;
    }
    std::cout<<"Return Success"<<std::endl;
    Parse parse;
    std::vector<std::string>tokens = parse.split(filename, '/');
    std::vector<std::string>goodFormat = parse.split(tokens[tokens.size() - 1], '.');
    std::cout<< "Output File: " << outdir + "/" + goodFormat[0] << std::endl;
    RandomPartition partition;
    partition.partition(i_idx, j_idx, values, n, nnz, nparts, outdir + "/" + goodFormat[0]);
    free(i_idx);
    free(j_idx);
    free(values);
    return 0;
}