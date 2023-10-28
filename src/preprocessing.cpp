#ifndef PREPROCESSING_HZY_CPP
#define PREPROCESSING_HZY_CPP
#include "hypergraph.h"

bool DelDegree1Vertex(HyperG & H);
bool DelCoveredEdge(HyperG & H);
bool DelCoveredEdge(HyperG & H);

bool InducedHyperG(HyperG & H, vector <size_t> & remain_vertex) {
    if(remain_vertex.size() == H.N)
        return 0;

    size_t new_N = remain_vertex.size();

    for(auto it = H.e.begin(); it != H.e.end(); ++it) {
        VertexSet temp;
        for(size_t i = 0; i < remain_vertex.size(); ++i) { // re-numbered
            size_t v = remain_vertex[i];
            if((*it).test(v))
                temp.Set(i);
        }
        (*it) = temp;
    }

    H.N = new_N;   
    return 1;
}

/*
    Delete degree 1 vertex:
        (a, b, c), (b, c, d)
        vertex a, d is degree 1 vertex and delete it will not effect the width
    return true if delete happens
*/

bool DelDegree1Vertex(HyperG & H) {
    size_t dgr[H.N];
    memset(dgr, 0, sizeof dgr);

    for(auto it = H.e.begin(); it != H.e.end(); ++it) {
        for(size_t i = 0; i < H.N; ++i)
            if((*it).test(i))
                dgr[i]++;            
    }

    vector <size_t> remain_vertex;

    for(size_t i = 0; i < H.N; ++i)
        if(dgr[i] > 1) 
            remain_vertex.push_back(i);

    return InducedHyperG(H, remain_vertex);
}

/*
    Delete the edge which can be covered by other edge:
        By greedy thought, it's always better to choose father set.
    return true if delete happens
*/

bool DelCoveredEdge(HyperG & H) {
    vector <VertexSet> res;
    bool flag = 0;

    for(size_t i = 0; i < H.e.size(); ++i) {
        bool fg = 1;
        for(size_t j = 0; j < H.e.size(); ++j) 
            if(i != j && ((H.e[i] & H.e[j]) == H.e[i])) { // covered
                if((H.e[i] == H.e[j]) && i > j)
                    continue;
                fg = 0;
                flag = 1;
                break;
            }
        if(fg)
            res.push_back(H.e[i]);
    }

    H.e = res;
    H.M = res.size();

    return flag;
}

/*
    The Vertex show in same Edge set is isomorphism,
    we only need to keep one
*/

bool DelISOVertex(HyperG & H) {
    VertexSet ve[H.N];

    size_t i = 0, j = 0;
    for(auto it = H.e.begin(); it != H.e.end(); ++it) {
        for(j = 0; j < H.N; ++j)
            if((*it).test(j))
                ve[j].Set(i);
        i++;
    }

    vector <size_t> remain_vertex;
    bool flag = 0;

    for(i = 0; i < H.N; ++i) {
        flag = 0;
        for(j = 0; j < H.N; ++j) {
            if(i == j)
                continue;
            if( /*(ve[i] & ve[j]) == ve[i] ||*/ ((ve[i] == ve[j]) && i > j) ) {
                flag = 1;
                break;
            }
        }
        if(!flag)
            remain_vertex.push_back(i);
    }

    return InducedHyperG(H, remain_vertex);

}


void Preprocessing(HyperG & H) {
    DelCoveredEdge(H);
    DelDegree1Vertex(H);
    DelISOVertex(H);

    while(1) { // until noting happened
        if(!DelCoveredEdge(H))
            break;
        if(!DelDegree1Vertex(H) && !DelISOVertex(H))
            break;
    }
}



#endif