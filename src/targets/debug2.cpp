#include <iostream>
#include <sstream>
#include <getopt.h>
#include <vector>

#include "../../../BallPivoting/src/Octree.h"
#include "../../../BallPivoting/src/OctreeIterator.h"
#include "../../../BallPivoting/src/Vertex.h"
#include "../../../BallPivoting/src/Mesher.h"
#include "../../../BallPivoting/src/FileIO.h"
#include "../MyRenderer.h"

#include <thread>
static void getAllVertices(OctreeNode *node, vector<CGL::Vector3D> &vertices) {
    if (node->getDepth() != 0) {
        for (int i = 0; i < 8; i++)
            if (node->getChild((unsigned int) i) != NULL)
                getAllVertices(node->getChild((unsigned int) i), vertices);
    }
    else if (node->getNpts() != 0) {
        Vertex_list::const_iterator iter;
        for (iter = node->points_begin(); iter != node->points_end(); ++iter) {
            vertices.push_back(CGL::Vector3D((*iter).x(), (*iter).y(), (*iter).z()));
        }
    }
}


/**
 * @brief main function for the ball pivoting reconstruction
 * @param argc
 * @param argv
 * @return 1 if the program exited successfully
 */
int main(int argc, char **argv) {
    //handling command line options
    int c;
    stringstream f;
    string infile, outfile, input_radii;
    unsigned int depth = 7;
    double radius = -1;
    int radius_flag = -1;
    int infile_flag = -1;
    int outfile_flag = -1;
    std::list<double> radii;
    int parallel_flag = -1;

    while ((c = getopt(argc, argv, "i:o:d:r:p")) != -1) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "missing_default_case"
        switch (c) {
            case 'i': {
                f.clear();
                f << optarg;
                f >> infile;
                infile_flag = 1;
                break;
            }
            case 'o': {
                f.clear();
                f << optarg;
                f >> outfile;
                outfile_flag = 1;
                break;
            }
            case 'd': {
                f.clear();
                f << optarg;
                f >> depth;
                break;
            }
            case 'p': {
                parallel_flag = 1;
                break;
            }
            case 'r': {
                input_radii = optarg;
                istringstream iss(input_radii, istringstream::in);
                while (iss >> radius) {
                    radii.push_back(radius);
                }
                radius_flag = 1;
                break;
            }
        }
#pragma clang diagnostic pop
    }

    if (infile_flag == -1) {
        std::cerr << "No input file given (use the -i option)" << std::endl;
        return EXIT_FAILURE;
    }

    if (outfile_flag == -1) {
        std::cerr << "No output file given (use the -o option)" << std::endl;
        return EXIT_FAILURE;
    }

    if (radius_flag == 1) {
        radii.sort();
        radius = radii.front();
    }

    time_t start, end;

    Octree octree;


    std::time(&start);
    bool ok;
    if (radius > 0) {
        ok = FileIO::readAndSortPoints(infile.c_str(), octree, radius);
    }
    else {
        octree.setDepth(depth);
        ok = FileIO::readAndSortPoints(infile.c_str(), octree);
    }
    if (!ok) {
        std::cerr << "Pb opening the file; exiting." << std::endl;
        return EXIT_FAILURE;
    }
    std::time(&end);

    std::cout << "Octree with depth " << octree.getDepth() << " created." << std::endl;
    std::cout << "Octree contains " << octree.getNpoints()
    << " points. The bounding box size is "
    << octree.getSize() << std::endl;
    std::cout << "Reading and sorting points in this octree took "
    << difftime(end, start) << " s." << std::endl;
    std::cout << "Octree statistics" << std::endl;
    octree.printOctreeStat();


    std::cout << "****** Reconstructing with radii " << std::flush;

    std::list<double>::const_iterator ri = radii.begin();
    while (ri != radii.end()) {
        std::cout << *ri << "; ";
        ++ri;
    }
    std::cout << "******" << std::endl;

    OctreeIterator iterator(&octree);

    if (radius > 0)
        iterator.setR(radius);

    std::time(&start);

    Mesher mesher(&octree, &iterator);

    // OUR CODE BEGIN

    vector<Facet *> *seedTriangles = new vector<Facet *>();
    vector<Facet *> *expansionTriangles = new vector<Facet *>();
    vector<Facet *> *fillHolesTriangles = new vector<Facet *>();

    auto renderOneFacet = [](Facet *facet) {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; ++i) {
            double x = facet->vertex((unsigned int) i)->x();
            double y = facet->vertex((unsigned int) i)->y();
            double z = facet->vertex((unsigned int) i)->z();
            glVertex3d(x, y, z);
        }
        glEnd();
    };

    auto initHook = []() {
//        glEnable(GL_DEPTH_TEST);
        return;
    };

    float a = 0.5;
    float d = (float) (0.5 / radii.size());
    function<void(void)> renderHook = [=, &a] {
        glColor4f(0.0, a, 0.0, 1.0);
        for (auto facet : *seedTriangles) {
            renderOneFacet(facet);
        }
        glColor4f(a, a, 0.0, 1.0);
        for (auto facet : *expansionTriangles) {
            renderOneFacet(facet);
        }
        glColor4f(a, 0.0, 0.0, 1.0);
        for (auto facet : *fillHolesTriangles) {
            renderOneFacet(facet);
        }
    };

    auto seedTriangleCallback = [&mesher, seedTriangles](Facet *facet) {
        seedTriangles->push_back(facet);
    };

    auto expandTriangulationCallback = [&mesher, expansionTriangles](Facet *facet) {
        expansionTriangles->push_back(facet);
    };

    auto fillHolesCallback = [&mesher, fillHolesTriangles](Facet *facet) {
        fillHolesTriangles->push_back(facet);
    };

    auto nextRadiusCallback = [&a, d] {
        a += d;
    };

    auto keyHook = [&mesher](char key) {
        mesher.stop = !mesher.stop;
    };

    mesher.seedTriangleCallback = seedTriangleCallback;
    mesher.expandTriangulationCallback = expandTriangulationCallback;
    mesher.fillHolesCallback = fillHolesCallback;
    mesher.nextRadiusCallback = nextRadiusCallback;

    CGL::MyRenderer *renderer = new CGL::MyRenderer();
    CGL::Viewer *viewer = new CGL::Viewer();

    renderer->renderHook = renderHook;
    renderer->keyEventHook = keyHook;
    renderer->initHook = initHook;

    vector<CGL::Vector3D> vertices;
    getAllVertices(octree.getRoot(), vertices);
    renderer->setBounds(vertices);
    viewer->set_renderer(renderer);
    viewer->init();

    auto startFn = [&mesher, &radii, parallel_flag]() {
        if(parallel_flag == 1)
            mesher.parallelReconstruct(radii);
        else
            mesher.reconstruct(radii);
        std::cout << "Filling holes" << std::endl;
        mesher.fillHoles();
        std::cout << "Done filling holes!" << std::endl;
    };

    std::thread pc2mThread(startFn);
    viewer->start();
    pc2mThread.join();


    // OUR CODE END

    if (parallel_flag == 1)
        mesher.parallelReconstruct(radii);
    else
        mesher.reconstruct(radii);
    std::time(&end);

    std::cout << "Reconstructed mesh: " << mesher.nVertices()
    << " vertices; " << mesher.nFacets() << " facets. " << std::endl;
    std::cout << mesher.nBorderEdges() << " border edges" << std::endl;
    std::cout << "Reconstructing the mesh took " << difftime(end, start)
    << "s." << std::endl;

    std::cout << "Filling the holes... " << std::flush;

    std::time(&start);
//    mesher.fillHoles();
    std::time(&end);

    std::cout << difftime(end, start) << " s." << std::endl;
    std::cout << "Final mesh: " << mesher.nVertices()
    << " vertices; " << mesher.nFacets() << " facets. " << std::endl;
    std::cout << mesher.nBorderEdges() << " border edges" << std::endl;

    if (!FileIO::saveMesh(outfile.c_str(), mesher)) {
        std::cerr << "Pb saving the mesh; exiting." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}