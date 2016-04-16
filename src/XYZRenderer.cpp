//
// Created by Michael Snowden on 4/15/16.
//

#include "XYZRenderer.h"

namespace CGL {

    void XYZRenderer::init(void) {
        // unimplemented
    }

    void XYZRenderer::render(void) {
        float *vertex = vertices;
        glBegin(GL_POINTS);
        for (long i = 0; i < nVertices; ++i) {
            glVertex3d(vertex[0] / 100, vertex[1] / 100.f, vertex[2] / 100.f);
            vertex += 3;
        }
        glEnd();
    }

    void XYZRenderer::resize(size_t w, size_t h) {
        // unimplemented
    }

    std::string XYZRenderer::name(void) {
        return "XYZRenderer";
    }

    std::string XYZRenderer::info(void) {
        return "Renders .xyz files as point clouds";
    }

    void XYZRenderer::load(std::string fname) {
        FILE *file = fopen(fname.c_str(), "r");
        if (!file) return;

        char line[256];
        nVertices = -1;
        fgets(line, sizeof(line), file);
        sscanf(line, "%li", &nVertices);
        if (nVertices == -1) return;

        free(vertices);
        vertices = (GLfloat *) malloc(sizeof(GLfloat) * nVertices * 3);

        GLfloat * vertex = vertices;
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%f %f %f", vertex, vertex + 1, vertex + 2);
            vertex += 3;
        }
        fclose(file);
    }
}