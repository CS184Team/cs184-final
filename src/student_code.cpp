/*
 * Student solution for UC Berkeley Project 2
 *
 * Implemented by ____ on ____.
 *
 */

#include "student_code.h"
#include "mutablePriorityQueue.h"

namespace CGL {

    // factorial
    int fact(int n) {
        if (n == 0) {
            return 1;
        }
        return n * fact(n - 1);
    }

    // binomial coefficient
    int nCr(int n, int k) {
        return fact(n) / (fact(k) * fact(n - k));
    }

    // bernstein polynomial
    // http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/curves-surfaces/slide_052
    double B(int n, int i, double t) {
        return nCr(n, i) * pow(t, i) * pow(1 - t, n - i);
    }

    void BezierPatch::preprocess() {
        // TODO Part 1.
        // TODO If you use the matrix form for Bezier patch evaluation, you will need to
        // TODO compute your matrices based on the 16 control points here.
        // TODO You will also need to define your matrices
        // TODO as member variables in the "BezierPatch" class.
        // TODO If you use De Casteljau's recursive algorithm, you will not need to do anything here.

    }

    Vector3D BezierPatch::evaluate(double u, double v) const {
        // TODO Part 1.
        // TODO Returns the 3D point whose parametric coordinates are (u, v) on the Bezier patch.
        // TODO Note that both u and v are within [0, 1].

        // http://cs184.eecs.berkeley.edu/cs184_sp16/lecture/curves-surfaces/slide_069
        Vector3D sum = Vector3D();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                const Vector3D b_ij = controlPoints[i][j];
                sum += b_ij * B(3, i, u) * B(3, j, v);
            }
        }
        return sum;
    }

    void BezierPatch::add2mesh(Polymesh *mesh) const {
        // TODO Part 1.
        // TODO Tessellate the given Bezier patch into triangles uniformly on a 8x8 grid(8x8x2=128 triangles) in parameter space.
        // TODO You will call your own evaluate function here to compute vertex positions of the tessellated triangles.
        // TODO The "addTriangle" function inherited from the "BezierPatchLoader" class may help you add triangles to the output mesh.

        // 0 to 1 inclusive 8 pixels
        double scale = 1.0 / 8;

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {

                double u0 = i * scale;
                double v0 = j * scale;
                double u1 = u0 + scale;
                double v1 = v0 + scale;

                const Vector3D p00 = evaluate(u0, v0);  // left bottom
                const Vector3D p01 = evaluate(u0, v1);  // left top
                const Vector3D p10 = evaluate(u1, v0);  // right bottom
                const Vector3D p11 = evaluate(u1, v1);  // right top

                addTriangle(mesh, p01, p11, p10);   // left top, right top, right bottom
                addTriangle(mesh, p10, p00, p01);   // right bottom, left bottom, left top
            }
        }
    }

    Vector3D Vertex::normal(void) const
    // TODO Part 2.
    // TODO Returns an approximate unit normal at this vertex, computed by
    // TODO taking the area-weighted average of the normals of neighboring
    // TODO triangles, then normalizing.
    {
        // TODO Compute and return the area-weighted unit normal.

        Vector3D normal;
        HalfedgeCIter h = halfedge();
        HalfedgeCIter h_orig = h;
        do {
            Vector3D v0 = h->vertex()->position;
            Vector3D v1 = h->next()->vertex()->position;
            Vector3D v2 = h->next()->next()->vertex()->position;

            normal += cross(v0 - v1, v0 - v2);

            h = h->twin()->next();
        } while (h != h_orig);
        return normal.unit();
    }

    EdgeIter HalfedgeMesh::flipEdge(EdgeIter e0) {
        // TODO Part 3.
        // TODO This method should flip the given edge and return an iterator to the flipped edge.

        if (e0->isBoundary()) {
            return e0;
        }

        HalfedgeIter h0 = e0->halfedge();
        HalfedgeIter h1 = h0->next();
        HalfedgeIter h2 = h1->next();
        HalfedgeIter h3 = h0->twin();
        HalfedgeIter h4 = h3->next();
        HalfedgeIter h5 = h4->next();
        HalfedgeIter h6 = h1->twin();
        HalfedgeIter h7 = h2->twin();
        HalfedgeIter h8 = h4->twin();
        HalfedgeIter h9 = h5->twin();

        VertexIter v0 = h0->vertex();
        VertexIter v1 = h3->vertex();
        VertexIter v2 = h6->vertex();
        VertexIter v3 = h8->vertex();

        EdgeIter e1 = h1->edge();
        EdgeIter e2 = h2->edge();
        EdgeIter e3 = h4->edge();
        EdgeIter e4 = h5->edge();

        FaceIter f0 = h0->face();
        FaceIter f1 = h3->face();

        //  assigning
        h0->next() = h1;
        h0->twin() = h3;
        h0->vertex() = v3;
        h0->edge() = e0;
        h0->face() = f0;

        h1->next() = h2;
        h1->twin() = h7;
        h1->vertex() = v2;
        h1->edge() = e2;
        h1->face() = f0;

        h2->next() = h0;
        h2->twin() = h8;
        h2->vertex() = v0;
        h2->edge() = e3;
        h2->face() = f0;

        h3->next() = h4;
        h3->twin() = h0;
        h3->vertex() = v2;
        h3->edge() = e0;
        h3->face() = f1;

        h4->next() = h5;
        h4->twin() = h9;
        h4->vertex() = v3;
        h4->edge() = e4;
        h4->face() = f1;

        h5->next() = h3;
        h5->twin() = h6;
        h5->vertex() = v1;
        h5->edge() = e1;
        h5->face() = f1;

        h6->next() = h6->next();    // doesn't change!
        h6->twin() = h5;
        h6->vertex() = v2;
        h6->edge() = e1;
        h6->face() = h6->face();    // doesn't change!

        h7->next() = h7->next();    // doesn't change!
        h7->twin() = h1;
        h7->vertex() = v0;
        h7->edge() = e2;
        h7->face() = h7->face();    // doesn't change!

        h8->next() = h8->next();    // doesn't change!
        h8->twin() = h2;
        h8->vertex() = v3;
        h8->edge() = e3;
        h8->face() = h8->face();    // doesn't change!

        h9->next() = h9->next(); // didn’t change, but set it anyway!
        h9->twin() = h4;
        h9->vertex() = v1;
        h9->edge() = e4;
        h9->face() = h9->face(); // didn’t change, but set it anyway!

        v0->halfedge() = h2;
        v1->halfedge() = h5;
        v2->halfedge() = h3;
        v3->halfedge() = h0;


        e0->halfedge() = h0;
        e1->halfedge() = h5;
        e2->halfedge() = h1;
        e3->halfedge() = h2;
        e4->halfedge() = h4;

        f0->halfedge() = h0;
        f1->halfedge() = h3;

        return e0;
    }

    VertexIter HalfedgeMesh::splitEdge(EdgeIter e0) {
        // TODO Part 4.
        // TODO This method should split the given edge and return an iterator to the newly inserted vertex.
        // TODO The halfedge of this vertex should point along the edge that was split, rather than the new edges.

        if (e0->isBoundary()) {
            return e0->halfedge()->vertex();
        }

        // Collect old elements

        HalfedgeIter h0 = e0->halfedge();
        HalfedgeIter h1 = h0->next();
        HalfedgeIter h2 = h1->next();
        HalfedgeIter h3 = h0->twin();
        HalfedgeIter h4 = h3->next();
        HalfedgeIter h5 = h4->next();
        HalfedgeIter h6 = h1->twin();
        HalfedgeIter h7 = h2->twin();
        HalfedgeIter h8 = h4->twin();
        HalfedgeIter h9 = h5->twin();

        VertexIter v0 = h0->vertex();
        VertexIter v1 = h3->vertex();
        VertexIter v2 = h6->vertex();
        VertexIter v3 = h8->vertex();

        EdgeIter e1 = h1->edge();
        EdgeIter e2 = h2->edge();
        EdgeIter e3 = h4->edge();
        EdgeIter e4 = h5->edge();

        FaceIter f0 = h0->face();
        FaceIter f1 = h3->face();

        // Allocate new elements

        HalfedgeIter h10 = newHalfedge();
        HalfedgeIter h11 = newHalfedge();
        HalfedgeIter h12 = newHalfedge();
        HalfedgeIter h13 = newHalfedge();
        HalfedgeIter h14 = newHalfedge();
        HalfedgeIter h15 = newHalfedge();

        VertexIter v4 = newVertex();

        EdgeIter e5 = newEdge();
        EdgeIter e6 = newEdge();
        EdgeIter e7 = newEdge();

        FaceIter f2 = newFace();
        FaceIter f3 = newFace();

        // Assign things

        h0->next() = h1;
        h0->twin() = h3;
        h0->vertex() = v4;
        h0->edge() = e0;
        h0->face() = f0;

        h1->next() = h10;
        h1->twin() = h6;
        h1->vertex() = v1;
        h1->edge() = e1;
        h1->face() = f0;

        h2->next() = h12;
        h2->twin() = h7;
        h2->vertex() = v2;
        h2->edge() = e2;
        h2->face() = f2;

        h3->next() = h11;
        h3->twin() = h0;
        h3->vertex() = v1;
        h3->edge() = e0;
        h3->face() = f1;

        h4->next() = h14;
        h4->twin() = h8;
        h4->vertex() = v0;
        h4->edge() = e3;
        h4->face() = f3;

        h5->next() = h3;
        h5->twin() = h9;
        h5->vertex() = v3;
        h5->edge() = e4;
        h5->face() = f1;

        h6->next() = h6->next();    // doesn't change!
        h6->twin() = h1;
        h6->vertex() = v2;
        h6->edge() = e1;
        h6->face() = h6->face();    // doesn't change!

        h7->next() = h7->next();    // doesn't change!
        h7->twin() = h2;
        h7->vertex() = v0;
        h7->edge() = e2;
        h7->face() = h7->face();    // doesn't change!

        h8->next() = h8->next();    // doesn't change!
        h8->twin() = h4;
        h8->vertex() = v3;
        h8->edge() = e3;
        h8->face() = h8->face();    // doesn't change!

        h9->next() = h9->next(); // didn’t change, but set it anyway!
        h9->twin() = h5;
        h9->vertex() = v1;
        h9->edge() = e4;
        h9->face() = h9->face(); // didn’t change, but set it anyway!

        h10->next() = h0;
        h10->twin() = h13;
        h10->vertex() = v2;
        h10->edge() = e5;
        h10->face() = f0;

        h11->next() = h5;
        h11->twin() = h14;
        h11->vertex() = v4;
        h11->edge() = e7;
        h11->face() = f1;

        h12->next() = h13;
        h12->twin() = h15;
        h12->vertex() = v0;
        h12->edge() = e6;
        h12->face() = f2;

        h13->next() = h2;
        h13->twin() = h10;
        h13->vertex() = v4;
        h13->edge() = e5;
        h13->face() = f2;

        h14->next() = h15;
        h14->twin() = h11;
        h14->vertex() = v3;
        h14->edge() = e7;
        h14->face() = f3;

        h15->next() = h4;
        h15->twin() = h12;
        h15->vertex() = v4;
        h15->edge() = e6;
        h15->face() = f3;

        v0->halfedge() = h4;
        v1->halfedge() = h1;
        v2->halfedge() = h2;
        v3->halfedge() = h5;
        v4->halfedge() = h0;
        v4->position = (v0->position + v1->position) / 2;

        e0->halfedge() = h0;
        e1->halfedge() = h1;
        e2->halfedge() = h2;
        e3->halfedge() = h4;
        e4->halfedge() = h5;
        e5->halfedge() = h10;
        e6->halfedge() = h12;
        e7->halfedge() = h11;

        f0->halfedge() = h0;
        f1->halfedge() = h3;
        f2->halfedge() = h2;
        f3->halfedge() = h4;

        return v4;
    }

    void Vertex::computeCentroid() {
        Size n = degree();
        double u = (n == 3) ? 3.0 / 16 : 3.0 / (8 * n);
        Vector3D original_position = this->position;
        Vector3D neigbor_position_sum;
        HalfedgeIter h = halfedge();
        HalfedgeIter h_orig = h;
        do {
            neigbor_position_sum += h->twin()->vertex()->position;
            h = h->twin()->next();
        } while (h != h_orig);

        this->newPosition = (1.0 - n * u) * original_position + u * neigbor_position_sum;
    }

    void MeshResampler::upsample(HalfedgeMesh &mesh)
    // TODO Part 5.
    // This routine should increase the number of triangles in the mesh using Loop subdivision.
    {
        // Each vertex and edge of the original surface can be associated with a vertex in the new (subdivided) surface.
        // Therefore, our strategy for computing the subdivided vertex locations is to *first* compute the new positions
        // using the connectity of the original (coarse) mesh; navigating this mesh will be much easier than navigating
        // the new subdivided (fine) mesh, which has more elements to traverse.  We will then assign vertex positions in
        // the new mesh based on the values we computed for the original mesh.

        // TODO Compute new positions for all the vertices in the input mesh, using the Loop subdivision rule,
        // TODO and store them in Vertex::newPosition. At this point, we also want to mark each vertex as being
        // TODO a vertex of the original mesh.
        for (auto vertex = mesh.verticesBegin(); vertex != mesh.verticesEnd(); ++vertex) {
            if (vertex->isBoundary()) continue;
            vertex->isNew = false;
            vertex->computeCentroid();
        }

        // TODO Next, compute the updated vertex positions associated with edges, and store it in Edge::newPosition.
        for (auto edge = mesh.edgesBegin(); edge != mesh.edgesEnd(); ++edge) {
            if (edge->halfedge()->isBoundary() || edge->halfedge()->twin()->isBoundary()) continue;
            edge->isNew = false;
            Vector3D a = edge->halfedge()->vertex()->position;
            Vector3D b = edge->halfedge()->twin()->vertex()->position;
            Vector3D c = edge->halfedge()->next()->twin()->vertex()->position;
            Vector3D d = edge->halfedge()->twin()->next()->twin()->vertex()->position;
            edge->newPosition = 3.0 / 8.0 * (a + b) + 1.0 / 8.0 * (c + d);
        }

        // TODO Next, we're going to split every edge in the mesh, in any order.  For future
        // TODO reference, we're also going to store some information about which subdivided
        // TODO edges come from splitting an edge in the original mesh, and which edges are new,
        // TODO by setting the flat Edge::isNew.  Note that in this loop, we only want to iterate
        // TODO over edges of the original mesh---otherwise, we'll end up splitting edges that we
        // TODO just split (and the loop will never end!)
        for (auto edge = mesh.edgesBegin(); edge != mesh.edgesEnd(); ++edge) {
            if (edge->halfedge()->isBoundary() || edge->halfedge()->twin()->isBoundary()) continue;
            if (!edge->isNew && !edge->halfedge()->vertex()->isNew && !edge->halfedge()->twin()->vertex()->isNew) {
                VertexIter m = mesh.splitEdge(edge);
                m->position = m->halfedge()->edge()->newPosition;
                m->isNew = true;
                m->halfedge()->next()->next()->edge()->isNew = true;
                m->halfedge()->twin()->next()->edge()->isNew = true;
            }
        }

        // TODO Now flip any new edge that connects an old and new vertex.
        for (auto edge = mesh.edgesBegin(); edge != mesh.edgesEnd(); ++edge) {
            if (edge->halfedge()->isBoundary() || edge->halfedge()->twin()->isBoundary()) continue;
            if (edge->isNew and (edge->halfedge()->vertex()->isNew xor edge->halfedge()->twin()->vertex()->isNew)) {
                mesh.flipEdge(edge);
            }
        }

        // TODO Finally, copy the new vertex positions into final Vertex::position.
        for (auto vertex = mesh.verticesBegin(); vertex != mesh.verticesEnd(); ++vertex) {
            if (vertex->isBoundary()) continue;
            if (!vertex->isNew) {
                vertex->position = vertex->newPosition;
            }
        }
    }

    // TODO Part 6.
    // TODO There's also some code you'll need to complete in "Shader/frag" file.

}
