#ifndef TD_HZY_H
#define TD_HZY_H
#define db double

#include "hypergraph.h"

void EnumOrderFHD(HyperG & H, vector <size_t> & V, Width & current_ans, Width & optimal_ans, vector <VertexSet> & E, VertexSet suffix);

/*
    Divide and conquer set method for fhd:
        H: given hypergraph
        Hstate: prefix and suffix, a state of divide and conquer
        V: divided vertex set, each iteration will be divided up
        optimal_ans: maintain the optimal answer from now on
        E: edge set, maintain connectivity and aggregation operation
        ckf: check process or not, if true then break when find a feasible solution

*/

void DivOrderFHD(HyperG & H, Hstate S, vector <size_t> & V, Width & optimal_ans, vector <VertexSet> & E, bool ckf, clock_t start_time);

db DPFHD(HyperG & H, FILE * f);



#endif