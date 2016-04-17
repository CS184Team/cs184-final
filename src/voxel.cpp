#include "voxel.h"

// Allen Zeng. April 16, 2016.

VoxelArray::VoxelArray(const vector<Vector3D> &cloud, double rho) : cloud(cloud), rho(rho) {
    double maxVal = std::numeric_limits<double>::max();
    low = CGL::Vector3D(maxVal, maxVal, maxVal);
    high = CGL::Vector3D(-maxVal, -maxVal, -maxVal);
    for (int i = 0; i < cloud.size(); ++i) {
        CGL::Vector3D vertex = cloud[i];
        double x = vertex.x;
        double y = vertex.y;
        double z = vertex.z;
        if (x < low.x) {
            low.x = x;
        }
        if (y < low.y) {
            low.y = y;
        }
        if (z < low.z) {
            low.z = z;
        }
        if (x > high.x) {
            high.x = x;
        }
        if (y > high.y) {
            high.y = y;
        }
        if (z > high.z) {
            high.z = z;
        }
    }
    double delta = 2.0 * rho;
    xlen = (int) ceil((high.x - low.x) / delta);
    ylen = (int) ceil((high.y - low.y) / delta);
    zlen = (int) ceil((high.z - low.z) / delta);

    int total_length = xlen * ylen * zlen;
    data.reserve((unsigned long) total_length);
    for (int i = 0; i < total_length; ++i) {
        data.push_back(vector<int>());
    }

    for (int i = 0; i < cloud.size(); i++) {
        data[get_voxel_index(cloud[i])].push_back(i);
    }
}

void VoxelArray::get_voxel_index(const Vector3D &xyz, int &xi, int &yi, int &zi) const {
    Vector3D path = (xyz - low) / (2.0 * rho);
    xi = (int) floor(path.x);
    yi = (int) floor(path.y);
    zi = (int) floor(path.z);
}

int VoxelArray::get_voxel_index(const Vector3D &xyz) const {
    int xi, yi, zi;
    get_voxel_index(xyz, xi, yi, zi);
    return flatten_index(xi, yi, zi);
}

int VoxelArray::flatten_index(int xi, int yi, int zi) const {
    return zi * (xlen * ylen) + yi * (xlen) + xi;
}

vector<vector<int> > VoxelArray::surrounding_voxels(const Vector3D &xyz) const {
    vector<vector<int> > voxels;
    voxels.reserve(27);

    int xi, yi, zi;
    get_voxel_index(xyz, xi, yi, zi);

    for (int xj = max(xi - 1, 0); xj <= min(xi + 1, xlen - 1); ++xj) {
        for (int yj = max(yi - 1, 0); yj <= min(yi + 1, ylen - 1); ++yj) {
            for (int zj = max(zi - 1, 0); zj <= min(zi + 1, zlen - 1); ++zj) {
                voxels.push_back(data[flatten_index(xj, yj, zj)]);
            }
        }
    }

    return voxels;
}

vector<int> VoxelArray::get_neighbor_indices(int central_cloud_idx) {
    Vector3D xyz = cloud[central_cloud_idx];
    vector<vector<int> > voxels = surrounding_voxels(xyz);
    vector<int> neighbors;
    for (int i = 0; i < voxels.size(); ++i) {
        for (int j = 0; j < voxels[i].size(); ++j) {
            int cloud_idx = voxels[i][j];
            if (cloud_idx != central_cloud_idx) {
                neighbors.push_back(cloud_idx);
            }
        }
    }
    sort(neighbors.begin(), neighbors.end(), [this, &xyz](int a, int b) {
        return (cloud[a] - xyz).norm2() < (cloud[b] - xyz).norm2();
    });
    return neighbors;
}