//#include "../Parser.h"
//#include "../MyRenderer.h"
//#include "../../../pc2m2/src/Vector.h"
//#include "../../../pc2m2/src/Vertex.h"
//#include "../../../pc2m2/src/Mesher.h"
//#include "../../../pc2m2/src/Facet.h"
//#include <thread>
//
//#define COLOR_BLUE glColor4f(0.012, 0.54, 0.61, 0.5);
//#define COLOR_YELLOW glColor4f(1.0, 0.68, 0.0, 0.5);
//#define COLOR_EXPAND glColor4f(1.0, 0.075, 0.0, 0.5);
//#define COLOR_GREEN glColor4f(0.0, 1.0, 0.0, 0.75);
//
//std::vector<Vector *> *convert(vector<Vector3D> vectors) {
//    std::vector<Vector *> *converted = new vector<Vector *>();
//    for (auto v : vectors) {
//        converted->push_back(new Vector(v.x, v.y, v.z));
//    }
//    return converted;
//}
//
//int main(int argc, char **argv) {
//    if (argc < 3) {
//        printf("Usage:\n\t%s <input.txt> <radius>\n", argv[0]);
//        return 1;
//    }
//    char *iname = argv[1];
//    Parser parser;
//    vector<Vector3D> vertices;
//    vector<Vector3D> normals;
//    parser.parseTxt(iname, vertices, normals);
//    vector<Facet *> *seedTriangles = new vector<Facet *>();
//    vector<Facet *> *expansionTriangles = new vector<Facet *>();
//    vector<Facet *> *fillHolesTriangles = new vector<Facet *>();
//
//    auto renderOneFacet = [](Facet &f) {
//        glBegin(GL_TRIANGLES);
//        for (int i = 0; i < 3; ++i) {
//            Vertex *vertex = f.getVertex(i);
//            double x = vertex->x();
//            double y = vertex->y();
//            double z = vertex->z();
//            glVertex3d(x, y, z);
//        }
//        glEnd();
//    };
//
//    auto initHook = []() {
//        return;
//    };
//
//    function<void(void)> renderHook = [=] {
//        COLOR_GREEN
//        for (auto f : *seedTriangles) {
//            renderOneFacet(*f);
//        }
//        COLOR_YELLOW
//        for (auto f : *expansionTriangles) {
//            renderOneFacet(*f);
//        }
//        COLOR_EXPAND
//        for (auto f : *fillHolesTriangles) {
//            renderOneFacet(*f);
//        }
//    };
//
//    vector<Vector *> *vertices2 = convert(vertices);
//    vector<Vector *> *normals2 = convert(normals);
//    vector<Vertex *> *pointCloud = new vector<Vertex *>();
//    for (int i = 0; i < vertices.size(); ++i) {
//        pointCloud->push_back(new Vertex((*vertices2)[i], (*normals2)[i]));
//    }
//
//    double radius;
//    std::sscanf(argv[2], "%lf", &radius);
//
//    Mesher *mesher = new Mesher(*pointCloud, radius);
//    MyRenderer *renderer = new MyRenderer();
//    Viewer *viewer = new Viewer();
//
//    auto keyHook = [mesher](char key) {
//        mesher->stop = !mesher->stop;
//    };
//
//    auto seedTriangleCallback = [mesher, seedTriangles](Facet *f) {
//        std::cout << "Triangle" << std::endl;
//        mesher->stop = true;
//        seedTriangles->push_back(f);
//    };
//
//    auto expandTriangulationCallback = [mesher, expansionTriangles](Facet *f) {
//        mesher->stop = true;
//        expansionTriangles->push_back(f);
//    };
//
//    auto fillHolesCallback = [mesher, fillHolesTriangles](Facet *f) {
//        fillHolesTriangles->push_back(f);
//    };
//
//    auto start = [mesher]() {
//        mesher->reconstruct();
//    };
//
//    mesher->seedTriangleCallback = seedTriangleCallback;
//    mesher->expandTriangulationCallback = expandTriangulationCallback;
//    mesher->fillHolesCallback = fillHolesCallback;
//    renderer->renderHook = renderHook;
//    renderer->keyEventHook = keyHook;
//    renderer->initHook = initHook;
//    renderer->setBounds(vertices);
//
//
//    viewer->set_renderer(renderer);
//    viewer->init();
//
//
//    std::thread pc2mThread(start);
//    viewer->start();
//    pc2mThread.join();
//
//    delete expansionTriangles;
//    delete seedTriangles;
//    return 0;
//}