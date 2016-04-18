//
// Created by Michael Snowden on 4/17/16.
//

#include "Parser.h"
#include "mesh.h"
#include "meshEdit.h"

static double *staticVertices;
static unsigned int *faces;
static int vertIdx;
static int faceIdx;

using namespace std;
using namespace CGL;

static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double value = ply_get_argument_value(argument);
    staticVertices[vertIdx] = value;
    ++vertIdx;
    return 1;
}

static int face_cb(p_ply_argument argument) {
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    if (value_index == 0 || value_index == 1 || value_index == 2) {
        unsigned int idx = (unsigned int) ply_get_argument_value(argument);
        faces[faceIdx] = idx;
        ++faceIdx;
    }
    return 1;
}

Parser::Parser() {

}

int Parser::parsePly(char *iname, vector<Vector3D> &vertices, vector<Vector3D> &normals) {
    p_ply ply = ply_open(iname, NULL, 0, NULL);
    if (!ply) {
        printf("[XYZ] Error: Could not open ifile (%s) for reading. Exiting\n", iname);
        return 0;
    }
    if (!ply_read_header(ply)) {
        printf("[XYZ] Error: Could not read ply header from ifile (%s). Exiting\n", iname);
        return 0;
    }

    long nVertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
    long nFaces = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

    staticVertices = (double *) malloc(sizeof(double) * nVertices * 3);
    faces = (unsigned int *) malloc(sizeof(unsigned int) * nFaces * 3);
    if (!ply_read(ply)) return 1;
    ply_close(ply);

    Polymesh *mesh = new Polymesh();

    mesh->vertices.reserve((unsigned long) nVertices);
    for (int i = 0; i < nVertices; ++i) {
        double *vertex = staticVertices + i * 3;
        mesh->vertices.push_back(Vector3D(vertex[0], vertex[1], vertex[2]));
    }
    mesh->polygons.reserve((unsigned long) nFaces);
    for (int i = 0; i < nFaces; ++i) {
        unsigned int *face = faces + i * 3;
        Polygon poly = Polygon();
        poly.vertex_indices.push_back(face[0]);
        poly.vertex_indices.push_back(face[1]);
        poly.vertex_indices.push_back(face[2]);
        mesh->polygons.push_back(poly);
    }

    MeshNode meshNode(*mesh);
    for (VertexIter vertex = meshNode.mesh.verticesBegin(); vertex != meshNode.mesh.verticesEnd(); ++vertex) {
        Vector3D position = vertex->position;
        double x = position[0];
        double y = position[1];
        double z = position[2];
        Vector3D normal = vertex->normal();
        normals.push_back(normal);
    }
    free(staticVertices);
    free(faces);
    staticVertices = NULL;
    faces = NULL;
    return 1;
}