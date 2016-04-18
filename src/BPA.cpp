//
// Created by Michael Snowden on 4/17/16.
//

#include "BPA.h"

BPA::BPA(vector<Vector3D> &cloud, double rho) : cloud(cloud), rho(rho), voxelArray(VoxelArray(cloud, rho)) {
//    voxelArray = VoxelArray(cloud, rho);
}

void BPA::findSeedTriangle(int pi, int &ti1, int &ti2, int &ti3) {

}