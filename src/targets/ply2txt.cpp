#include "../Parser.h"
#include "CGL/CGL.h"


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage:\n\t%s <input.ply> <output.txt>\n", argv[0]);
    }
    char *ifile = argv[1];
    char *ofile = argv[2];
    Parser parser;
    std::vector<CGL::Vector3D> vertices;
    std::vector<CGL::Vector3D> normals;
    if (!parser.parsePly(ifile, vertices, normals)) {
        printf("[ply2txt] Error parsing %s. Exiting.\n", ifile);
        return 1;
    }
    if (!parser.writeTxt(ofile, vertices, normals)) {
        printf("[ply2txt] Error writing to %s", ofile);
        return 1;
    }
    printf("[ply2txt] Successfuly wrote %s to %s!\n", ifile, ofile);
    return 0;
}