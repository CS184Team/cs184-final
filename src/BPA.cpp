//
// Created by Michael Snowden on 4/17/16.
//

#include "BPA.h"

BPA::BPA(vector<Vector3D> &cloud, vector<Vector3D> &normals, double rho) : cloud(cloud), normals(normals), rho(rho), voxelArray(VoxelArray(cloud, rho)) {}

void BPA::findSeedTriangle(int pi, int &ti1, int &ti2, int &ti3) {
	vector<int> neighbor_indices = voxelArray.get_neighbor_indices(pi);
	for (int qi = 0; qi < neighbor_indices.size(); ++qi) {
		for (int si = qi + 1; si < neighbor_indices.size(); ++si) {
			Vector3D p, q, s;
			p = cloud[pi];
			q = cloud[qi];
			s = cloud[si];
			
		}
	}
}