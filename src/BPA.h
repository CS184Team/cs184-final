//
// Created by Michael Snowden on 4/16/16.
//

#ifndef COLLADAVIEWER_BPA_H
#define COLLADAVIEWER_BPA_H

#include "CGL/CGL.h"

using namespace CGL;
using namespace std;

class Vertex {
public:
    Vector3D position;
    Vector3D normal;
    bool in_reconstructed_triangulation;
    int index;
};

class OctTree {
public:
    ~OctTree();
    OctTree(Vertex &vertex);
    Vertex &vertex;
    OctTree *children[8];
    void add(Vertex &vertex);
};

class BPA {
public:
    void bpa();

    void find_seed_triangle(int &i, int &j, int &k);

    double row;

    Vertex &get_unused_vertex();

    vector<Vertex &> get_closest_neighbors(Vertex &vertex);

    Vector3D normal(Vertex &a, Vertex &b, Vertex &c);
};


#endif //COLLADAVIEWER_BPA_H
