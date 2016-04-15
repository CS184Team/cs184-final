/* 
 * File:   BezierPatch.cpp
 * Author: swl
 * 
 * Created on January 30, 2016, 4:59 PM
 */

#include "bezierPatch.h"
#include <unordered_map>
#include <unordered_set>

namespace CGL {

    void BezierPatch::addTriangle(Polymesh* mesh,
            const Vector3D& v0, const Vector3D& v1, const Vector3D& v2) const {
        size_t base = mesh->vertices.size();
        mesh->vertices.push_back(v0);
        mesh->vertices.push_back(v1);
        mesh->vertices.push_back(v2);
        Polygon poly;
        poly.vertex_indices.push_back(base);
        poly.vertex_indices.push_back(base + 1);
        poly.vertex_indices.push_back(base + 2);
        mesh->polygons.push_back(poly);
    }

    void BezierPatch::loadControlPoints(FILE* file) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                fscanf(file, "%lf %lf %lf", &controlPoints[i][j].x, &controlPoints[i][j].y,
                        &controlPoints[i][j].z);
            }
        }
        preprocess();
    }

}