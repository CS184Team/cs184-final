#include "CGL/CGL.h"

#include "../collada.h"
#include "../meshEdit.h"
#include "../bezierPatch.h"
#include "../mergeVertices.h"
#include "../shaderUtils.h"
#include "../rply.h"

#include <iostream>

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Collada Viewer] " << s << endl;
#define DEBUG 0

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