//
// Created by Michael Snowden on 4/17/16.
//

#ifndef COLLADAVIEWER_BPA_H
#define COLLADAVIEWER_BPA_H

#include "voxel.h"

using namespace std;
using namespace CGL;

class BPA {
public:
    BPA(vector<Vector3D> &cloud, vector<Vector3D> &normals, double rho);
    void findSeedTriangle(int pi, int &ti1, int &ti2, int &ti3);
private:
    vector<Vector3D> &cloud;
    vector<Vector3D> &normals;
    double rho;
    VoxelArray voxelArray;
};


#endif //COLLADAVIEWER_BPA_H
