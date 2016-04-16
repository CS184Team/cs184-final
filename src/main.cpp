#include "CGL/CGL.h"

#include "collada.h"
#include "meshEdit.h"
#include "bezierPatch.h"
#include "mergeVertices.h"
#include "shaderUtils.h"
#include "XYZRenderer.h"

#include <iostream>

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Collada Viewer] " << s << endl;

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
    } else if (path_str.substr(path_str.length() - 4, 4) == ".xyz") {
//        Camera *cam = new Camera();
//        cam->type = CAMERA;
//        Node node;
//        node.instance = cam;
//        scene->nodes.push_back(node);
//        Polymesh *mesh = new Polymesh();
//
//        FILE *file = fopen(path, "r");
//        if (!file) return -1;
//
//        char line[256];
//        long nVertices = -1;
//        fgets(line, sizeof(line), file);
//        sscanf(line, "%li", &nVertices);
//        if (nVertices == -1) return -1;
//
//        std::vector<Vector3D> vertices;
//        vertices.reserve((unsigned long) nVertices);
//        while (fgets(line, sizeof(line), file)) {
//            GLfloat x, y, z;
//            sscanf(line, "%f %f %f", &x, &y, &z);
//            vertices.push_back(Vector3D(x, y, z));
//        }
//        fclose(file);
//
//        mesh->vertices = vertices;
//        mesh->type = POLYMESH;
//        node.instance = mesh;
//        scene->nodes.push_back(node);
        return -1;
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

    // create viewer
    Viewer viewer = Viewer();

    // create collada_viewer
    MeshEdit *collada_viewer = new MeshEdit();
//    XYZRenderer* renderer = new XYZRenderer();

    // set collada_viewer as renderer
    viewer.set_renderer(collada_viewer);
//    viewer.set_renderer(renderer);

    // init viewer
    viewer.init();

    // load tests
    if (argc == 2) {
        if (loadFile(collada_viewer, argv[1]) < 0) exit(0);
//
    } else {
        msg("Usage: collada_viewer <path to scene file>");
        exit(0);
    }
//    renderer->load(argv[1]);

    // start viewer
    viewer.start();

    return 0;
}
