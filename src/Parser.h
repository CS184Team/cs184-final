//
// Created by Michael Snowden on 4/17/16.
//

#ifndef COLLADAVIEWER_FILEIO_H
#define COLLADAVIEWER_FILEIO_H

#include "CGL/CGL.h"
#include "rply.h"
#include <utility>

using namespace CGL;
using namespace std;

typedef struct {
	int v1;
	int v2;
	int v3;
} Triangle;

class Parser {
public:
    Parser();
    int parsePly(char *ifile, vector<Vector3D> &vertices, vector<Vector3D> &normals);
    int parsePlyFaces(char *ifile, vector<Vector3D> &vertices, vector<Triangle> &triangles);
    int parsePlyLines(char *ifile, vector<Vector3D> &vertices, vector<pair<int, int> > &lines);
    int writeTxt(char *ofile, vector<Vector3D> &vertices, vector<Vector3D> &normals);
    int parseTxt(const char *ifile, vector<Vector3D> &vertices, vector<Vector3D> &normals);
private:

};

#endif //COLLADAVIEWER_FILEIO_H
