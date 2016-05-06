#include "CGL/CGL.h"
#include "../MyRenderer.h"

using namespace std;
using namespace CGL;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: \n\ttxtview <file.txt>\n");
        return 1;
    }

    Viewer viewer = Viewer();
    MyRenderer *xyzRenderer = new MyRenderer();
    viewer.set_renderer(xyzRenderer);
    viewer.init();
    char *xyz_file = argv[1];
    xyzRenderer->load(xyz_file);
    viewer.start();

    return 0;
}