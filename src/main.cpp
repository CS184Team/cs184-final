#include "CGL/CGL.h"

#include "collada.h"
#include "meshEdit.h"
#include "bezierPatch.h"
#include "mergeVertices.h"
#include "shaderUtils.h"
#include "rply.h"

#include <iostream>

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Collada Viewer] " << s << endl;
#define DEBUG 0

static double *vertices;
static unsigned int *faces;
static int vertIdx;
static int faceIdx;

using namespace std;
using namespace CGL;

static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    double value = ply_get_argument_value(argument);
    vertices[vertIdx] = value;
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


int loadFile(MeshEdit *collada_viewer, const char *path) {

    Scene *scene = new Scene();

    std::string path_str = path;
    if (path_str.substr(path_str.length() - 4, 4) == ".dae") {
        if (ColladaParser::load(path, scene) < 0) {
            delete scene;
            return -1;
        }
    } else if (path_str.substr(path_str.length() - 4, 4) == ".bez") {
        Camera *cam = new Camera();
        cam->type = CAMERA;
        Node node;
        node.instance = cam;
        scene->nodes.push_back(node);
        Polymesh *mesh = new Polymesh();

        FILE *file = fopen(path, "r");
        int n = 0;
        fscanf(file, "%d", &n);
        for (int i = 0; i < n; i++) {
            BezierPatch patch;
            patch.loadControlPoints(file);
            patch.add2mesh(mesh);
            mergeVertices(mesh);
        }
        fclose(file);

        mesh->type = POLYMESH;
        node.instance = mesh;
        scene->nodes.push_back(node);
    } else {
        return -1;
    }

    collada_viewer->load(scene);

    GLuint tex = makeTex("envmap/asdf.png");
    if (!tex) tex = makeTex("../envmap/asdf.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex);
    glActiveTexture(GL_TEXTURE2);

    return 0;
}

int main(int argc, char **argv) {
    if (argc > 2) {
        FILE *ofile;
        char *iname = argv[1];
        char *oname = argv[2];

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
        long nFaces = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

        vertices = (double *) malloc(sizeof(double) * nVertices * 3);
        faces = (unsigned int *) malloc(sizeof(unsigned int) * nFaces * 3);

        if (!ply_read(ply)) return 1;
        ply_close(ply);

        Polymesh *mesh = new Polymesh();

        mesh->vertices.reserve((unsigned long) nVertices);
        for (int i = 0; i < nVertices; ++i) {
            double *vertex = vertices + i * 3;
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

        ofile = fopen(oname, "w");
        if (!ofile) {
            printf("[XYZ] Error: Could not open ofile (%s) for writing. Exiting\n", oname);
            return 1;
        }

        fprintf(ofile, "%li\n", nVertices);
        MeshNode meshNode(*mesh);
        for (VertexIter vertex = meshNode.mesh.verticesBegin(); vertex != meshNode.mesh.verticesEnd(); ++vertex) {
            Vector3D position = vertex->position;
            double x = position[0];
            double y = position[1];
            double z = position[2];
            Vector3D normal = vertex->normal();
            double nx = normal[0];
            double ny = normal[1];
            double nz = normal[2];
            fprintf(ofile, "%f %f %f %f %f %f\n", x, y, z, nx, ny, nz);
        }
        free(vertices);
        free(faces);
        vertices = NULL;
        faces = NULL;
        fclose(ofile);
        if (argc == 3) {
            return 0;
        }
        Camera *cam = new Camera();
        cam->type = CAMERA;
        Node node;
        node.instance = cam;
        Scene *scene = new Scene();
        scene->nodes.push_back(node);

        mesh->type = POLYMESH;
        node.instance = mesh;
        scene->nodes.push_back(node);
    }

    // create viewer
    Viewer viewer = Viewer();

    // create collada_viewer
    MeshEdit *collada_viewer = new MeshEdit();

    // set collada_viewer as renderer
    viewer.set_renderer(collada_viewer);

    // init viewer
    viewer.init();

    // load tests
    if (argc == 2) {
        if (loadFile(collada_viewer, argv[1]) < 0) exit(0);
    } else if (argc == 4) {

    } else {
        msg("Usage: collada_viewer <path to scene file>");
        exit(0);
    }
    // start viewer
    viewer.start();

    return 0;
}
