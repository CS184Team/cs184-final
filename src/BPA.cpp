//
// Created by Michael Snowden on 4/16/16.
//

#include "BPA.h"

OctTree::OctTree(Vertex &vertex) : vertex(vertex) { };

void OctTree::add(Vertex &vertex) {
    bool x = vertex.position.x > this->vertex.position.x;
    bool y = vertex.position.y > this->vertex.position.y;
    bool z = vertex.position.z > this->vertex.position.z;
    int idx = x + y << 1 + z << 2;
    if (children[idx] == NULL) {
        children[idx] = new OctTree(vertex);
    } else {
        children[idx]->add(vertex);
    }
}

void BPA::bpa() {
//  while (true)
//      if (v_k = ball_pivot(e_ij) and (not_used(v_k) || on_front(v_k)))
//          output_triangle(t_ikj)
//          join(eij, vk, F)
//          if (e_ki in F)
//              glue(e_ik, e_ki, F)
//          if (e_jk in F)
//              glue(e_kj, e_jk, F)
//      else
//          mark_as_boundary(e_ij)
//      if (t_ijk = find_seed_triangle())
//          output_triangle(t_ijk)
//          insert_edge(e_ij, F)
//          insert_edge(e_jk, F)
//          insert_edge(e_ki, F)
//      else
//          return
}


void BPA::find_seed_triangle(int &i, int &j, int &k) {
    // Pick any point v_a not yet used by the reconstructed triangulation.
    Vertex &v_a = get_unused_vertex();
    // Consider all pairs of points v_b, v_c in its neighborhood in order of distance from v_a
    vector<Vertex &> neighbors = get_closest_neighbors(v_a);
    for (int a_i = 0; a_i < neighbors.size(); ++a_i) {
        for (int b_i = a_i + 1; b_i < neighbors.size(); ++b_i) {
            // Build potential seed triangle (v_a, v_b, v_c)
            auto v_b = neighbors[a_i];
            auto v_c = neighbors[b_i];
            Vector3D normal = normal(v_a, v_b, v_c);
            // Check that the triangle normal is consistent with the vertex normals, i.e. pointing outward.
            if (dot(normal, v_a.normal) > 0 && dot(normal, v_b.normal) > 0 && dot(normal, v_b.normal) > 0) {
                // Test that a row-ball with center in the outward halfspace touches
                // all three vertices and contains no other data point
                if ()
            }
        }
    }
}