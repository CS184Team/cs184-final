//
// Created by Michael Snowden on 4/17/16.
//
#include "../Parser.h"

int main(int argc, char **argv) {
    Parser parser;
    vector<Vector3D> vertices;
    vector<Vector3D> normals;
    parser.parsePly(argv[1], vertices, normals);
}