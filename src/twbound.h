#ifndef TWBOUND_HZY_H
#define TWBOUND_HZY_H

#include "hypergraph.h"
#include "SCsolver.h"

db FHW_ub(VertexSet V, vector <VertexSet> E, SCsolver & Sv);
db FHW_lb(VertexSet V, vector <VertexSet> E, SCsolver & Sv);

#endif