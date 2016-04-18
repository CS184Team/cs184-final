#include "CGL/CGL.h"
#include "../XYZRenderer.h"

using namespace std;
using namespace CGL;

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: \n\txyzview <file.xyz>\n");
        return 1;
    }

    Viewer viewer = Viewer();
    XYZRenderer *xyzRenderer = new XYZRenderer();
    viewer.set_renderer(xyzRenderer);
    viewer.init();
    char *xyz_file = argv[1];
    xyzRenderer->load(xyz_file);
    viewer.start();

    return 0;
}
