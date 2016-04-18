//
// Created by Michael Snowden on 4/16/16.
//
#include "../voxel.h"
#include "../rply.h"
#include "../Parser.h"
#include <iostream>

using namespace std;
using namespace CGL;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage:\n\t./voxeltest <text.ply>\n");
        return 1;
    }

    vector<Vector3D> cloud;
    vector<Vector3D> normals;
    Parser parser;
    if (!parser.parsePly(argv[1], cloud, normals)) {
        printf("Something with wrong parsing %s. Exiting.\n", argv[1]);
        return 1;
    }

    double rho = 0.0001;

    VoxelArray voxelArray(cloud, rho);

    int stepMod = (int) (cloud.size() / 20.0);

    printf("<percent of cloud points tested> <num neighbors to this cloud point>\n");
    printf("<distance to first neighbor squared (should be smaller than next line>\n");
    printf("<distance to last neighbor squared (should be greater than previous line>\n");

    for (int i = 0; i < cloud.size(); ++i) {
        Vector3D test = cloud[i];
        vector<int> neighbor_indices = voxelArray.get_neighbor_indices(i);
        if (i % stepMod == 0) {
            printf("%d%%, %lu\n", (int) ((100.0 * i) / cloud.size()), neighbor_indices.size());
        }
        for (int j = 0; j < neighbor_indices.size(); ++j) {
            Vector3D neighbor = cloud[neighbor_indices[j]];
            if (i % stepMod == 0 && (j == 0 || j == neighbor_indices.size() - 1)) {
                printf("%f\n", (neighbor - test).norm2());
            }
        }
    }
}