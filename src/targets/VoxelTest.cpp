//
// Created by Michael Snowden on 4/16/16.
//
#include "../voxel.h"
#include "../rply.h"
#include <iostream>

using namespace std;
using namespace CGL;

static double *vertices;
static int vertIdx;

static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double value = ply_get_argument_value(argument);
    vertices[vertIdx] = value;
    ++vertIdx;
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage:\n\t./voxeltest <text.ply>\n");
        return 1;
    }

    char *iname = argv[1];

    p_ply ply = ply_open(iname, NULL, 0, NULL);
    if (!ply) {
        printf("[XYZ] Error: Could not open ifile (%s) for reading. Exiting\n", iname);
        return 1;
    }
    if (!ply_read_header(ply)) {
        printf("[XYZ] Error: Could not read ply header from ifile (%s). Exiting\n", iname);
        return 1;
    }

    long nVertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);

    vertices = (double *) malloc(sizeof(double) * nVertices * 3);

    if (!ply_read(ply)) return 1;
    ply_close(ply);

    vector<Vector3D> cloud;
    for (int i = 0; i < nVertices; ++i) {
        double *vertex = vertices + i * 3;
        cloud.push_back(Vector3D(vertex[0], vertex[1], vertex[2]));
    }

    double rho = 0.001;

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

    free(vertices);
    vertices = NULL;
}