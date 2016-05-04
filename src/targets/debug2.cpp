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

    vector<vector<Facet *> > seedTriangles;
    vector<vector<Facet *> > expansionTriangles;
    vector<vector<Facet *> > fillHolesTriangles;
    seedTriangles.push_back(vector<Facet *>());
    expansionTriangles.push_back(vector<Facet *>());
    fillHolesTriangles.push_back(vector<Facet *>());

    auto renderOneFacet = [&](Facet *facet) {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 3; ++i) {
            Vertex *vertex = facet->vertex((unsigned int) i);
            double x = vertex->x();
            double y = vertex->y();
            double z = vertex->z();
            double nx = vertex->nx();
            double ny = vertex->ny();
            double nz = vertex->nz();
            glNormal3d(nx, ny, nz);
            glVertex3d(x, y, z);
        }
        glEnd();
    };

    auto initHook = []() {
        glEnable(GL_DEPTH_TEST);
        return;
    };

    float d = (float) (0.5 / radii.size());

    auto renderFacets = [&](vector<vector<Facet *> > &collection, double r, double g, double b) {
        for (int i = 0; i < collection.size(); ++i) {
            glColor3d(r, g, b);
            for (int j = 0; j < collection[i].size(); ++j) {
                renderOneFacet(collection[i][j]);
            }
            r += d;
            g += d;
            b += d;
        }
    };

    function<void(void)> renderHook = [&] {
        renderFacets(seedTriangles, 0.0, 0.5, 0.0);
        renderFacets(expansionTriangles, 0.5, 0.5, 0.0);
        renderFacets(fillHolesTriangles, 0.5, 0.0, 0.0);
    };

    auto seedTriangleCallback = [&](Facet *facet) {
        seedTriangles.back().push_back(facet);
    };

    auto expandTriangulationCallback = [&](Facet *facet) {
        expansionTriangles.back().push_back(facet);
    };

    auto fillHolesCallback = [&](Facet *facet) {
        fillHolesTriangles.back().push_back(facet);
    };

    auto nextRadiusCallback = [&]() {
        seedTriangles.push_back(vector<Facet *>());
        expansionTriangles.push_back(vector<Facet *>());
        fillHolesTriangles.push_back(vector<Facet *>());
    };

    auto keyHook = [&](char key) {
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

    auto startFn = [&]() {
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