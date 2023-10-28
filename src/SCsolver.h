#ifndef SCSOLVER_HZY_H
#define SCSOLVER_HZY_H

#include "hypergraph.h"

struct SCsolver {
    vector < VertexSet > E;
    map <VertexSet, double> res;

    SCsolver() {}
    SCsolver(vector < VertexSet > & _E) {
        E = _E;
    }

    db solve(VertexSet S);  
};

double calc_width(HyperG & H, vector <size_t> o, vector <VertexSet> E, double * w);
double fractional_set_cover(size_t n, vector< VertexSet > & E, VertexSet e);
double FSC_LP(vector< VertexSet > & E, VertexSet e);

#endif