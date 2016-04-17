//
// Created by Michael Snowden on 4/16/16.
//

#ifndef COLLADAVIEWER_VOXEL_H
#define COLLADAVIEWER_VOXEL_H

#include "CGL/CGL.h"

using namespace std;
using namespace CGL;

class VoxelArray {
public:
    // Pass in the point cloud, bounding box low and high, and rho
    VoxelArray(const vector<Vector3D> &cloud, double rho);

    // Returns a list of nearest points, sorted by distance to xyz (closest first)
    vector<int> get_neighbor_indices(int central_cloud_idx);

private:
    vector<vector<int> > data;
    double rho;
    int xlen;
    int ylen;
    int zlen;
    Vector3D low;
    Vector3D high;

    // Returns a list of the neighboring voxels (list of lists of point cloud indices)
    // Includes the local voxel
    // Maximum of 27 voxels, minimum of 8 voxels (corner)
    vector<vector<int> > surrounding_voxels(const Vector3D &xyz) const;

    void get_voxel_index(const Vector3D &xyz, int &xi, int &yi, int &zi) const;

    const vector<Vector3D> &cloud; // Reference to point cloud
    
    int get_voxel_index(const Vector3D &xyz) const;

    int flatten_index(int xi, int yi, int zi) const;
};

#endif //COLLADAVIEWER_VOXEL_H
