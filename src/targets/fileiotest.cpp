#include "../Parser.h"

int main(int argc, char **argv) {
	char *file = argv[1];
	Parser parser;
    vector<Vector3D> vertices;
    vector<Vector3D> normals;
    if (!parser.parsePly(file, vertices, normals)) {
    	printf("[FileIO Test] There was an error parsing the file: %s. Exiting.\n", file);
    	return 1;
    }
    int nVertices = vertices.size();
    int nNormals = normals.size();
    printf("[FileIO Test] #vertices, #normals: %d %d\n", nVertices, nNormals);
    printf("[FileIO Test] #vertices = #normals? %d\n", nVertices == nNormals);
    if (nVertices == nNormals) {
    	printf("[FileIO Test] This test passes. The number of vertices and normals are equal!\n");
    }
    return 0;
}