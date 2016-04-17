#include "CGL/CGL.h"
#include <vector>

// Allen Zeng. April 16, 2016.

using namespace std;

static int clamp(int val, int low, int high) {
	return max(low, min(val, high));
}

namespace CGL {
	class Sorter {
	public:
		Vector3D xyz;
		bool operator()(Vector3D a, Vector3D b);
		Sorter(Vector3D xyz);
	};

	class Voxel {
	public:
		Voxel();
		Voxel(int x_idx, int y_idx, int z_idx, Vector3D center, vector<int> points);
		int x_idx;
		int y_idx;
		int z_idx;
		Vector3D center;

		// List of point cloud indices
		vector<int> points;
	};

	class VoxelArray {
	public:
		// Pass in the point cloud, bounding box low and high, and rho
		VoxelArray(vector<Vector3D> &cloud, Vector3D low, Vector3D high, double rho);
		~VoxelArray();

		// Returns a list of voxels which correspond to
		// the points contained within the voxel at xyz
		Voxel nearby_points(Vector3D xyz);

		// Returns a list of voxel indices, which correspond to the voxels surrounding
		// the point at xyz. Includes the local voxel
		// vector<int> surrounding_voxel_indices(Vector3D xyz);

		// Returns a list of the 27 neighboring voxels (list of lists of point cloud indices)
		// Includes the local voxel
		vector<Voxel> surrounding_voxels(Vector3D xyz);

		// Returns a list of nearest points, sorted
		vector<Vector3D> get_neighbors(Vector3D xyz);

	private:
		vector<Vector3D> &cloud;	// Reference to point cloud
		vector<vector<vector<Voxel> > > data;
		double delta;
		int xlen;
		int ylen;
		int zlen;
	};

Voxel::Voxel(int x_idx, int y_idx, int z_idx, Vector3D center, vector<int> points) : x_idx(x_idx), y_idx(y_idx), z_idx(z_idx), center(center), points(points) {};

VoxelArray::VoxelArray(vector<Vector3D> &cloud, Vector3D low, Vector3D high, double rho) : cloud(cloud) {
	delta = 2.0 * rho;
	xlen = ceil((high.x - low.x) / delta);
	ylen = ceil((high.y - low.y) / delta);
	zlen = ceil((high.z - low.z) / delta);

	data.reserve(xlen);
	for (int x_idx = 0; x_idx < xlen; ++x_idx) {
		vector<vector<Voxel> > square;
		square.reserve(ylen);
		for (int y_idx = 0; y_idx < ylen; ++y_idx) {
			vector<Voxel> row;
			row.reserve(zlen);
			for (int z_idx = 0; z_idx < zlen; ++z_idx) {
				double x = x_idx * delta + rho;
				double y = y_idx * delta + rho;
				double z = z_idx * delta + rho;
				Vector3D center(x, y, z);
				vector<int> points;
				Voxel voxel(x_idx, y_idx, z_idx, center, points);
				row.push_back(voxel);
			}
			square.push_back(row);
		}
		data.push_back(square);
	}

	for (int i = 0; i < cloud.size(); i++) {
		int x_idx = floor(cloud[i].x / delta);
		int y_idx = floor(cloud[i].y / delta);
		int z_idx = floor(cloud[i].z / delta);

		data[x_idx][y_idx][z_idx].points.push_back(i);
	}
}

VoxelArray::~VoxelArray() {
}

Voxel VoxelArray::nearby_points(Vector3D xyz) {
	int x = floor(xyz.x / delta);
	int y = floor(xyz.y / delta);
	int z = floor(xyz.z / delta);
	return data[x][y][z];
}

// vector<int> VoxelArray::surrounding_voxel_indices(Vector3D xyz) {
// 	int x = floor(xyz.x / delta);
// 	int y = floor(xyz.y / delta);
// 	int z = floor(xyz.z / delta);

// 	vector<int> indices;
// 	indices.push_back((x-1) + xlen * ( (y-1) + ylen * (z-1) ));		// 1
// 	indices.push_back( x    + xlen * ( (y-1) + ylen * (z-1) ));		// 2
// 	indices.push_back((x+1) + xlen * ( (y-1) + ylen * (z-1) ));		// 3
// 	indices.push_back((x-1) + xlen * (  y    + ylen * (z-1) ));		// 4
// 	indices.push_back( x    + xlen * (  y    + ylen * (z-1) ));		// 5
// 	indices.push_back((x+1) + xlen * (  y    + ylen * (z-1) ));		// 6
// 	indices.push_back((x-1) + xlen * ( (y+1) + ylen * (z-1) ));		// 7
// 	indices.push_back( x    + xlen * ( (y+1) + ylen * (z-1) ));		// 8
// 	indices.push_back((x+1) + xlen * ( (y+1) + ylen * (z-1) ));		// 9
// 	indices.push_back((x-1) + xlen * ( (y-1) + ylen *  z    ));		// 10
// 	indices.push_back( x    + xlen * ( (y-1) + ylen *  z    ));		// 11
// 	indices.push_back((x+1) + xlen * ( (y-1) + ylen *  z    ));		// 12
// 	indices.push_back((x-1) + xlen * (  y    + ylen *  z    ));		// 13
// 	indices.push_back( x    + xlen * (  y    + ylen *  z    ));		// 14
// 	indices.push_back((x+1) + xlen * (  y    + ylen *  z    ));		// 15
// 	indices.push_back((x-1) + xlen * ( (y+1) + ylen *  z    ));		// 16
// 	indices.push_back( x    + xlen * ( (y+1) + ylen *  z    ));		// 17
// 	indices.push_back((x+1) + xlen * ( (y+1) + ylen *  z    ));		// 18
// 	indices.push_back((x-1) + xlen * ( (y-1) + ylen * (z+1) ));		// 19
// 	indices.push_back( x    + xlen * ( (y-1) + ylen * (z+1) ));		// 20
// 	indices.push_back((x+1) + xlen * ( (y-1) + ylen * (z+1) ));		// 21
// 	indices.push_back((x-1) + xlen * (  y    + ylen * (z+1) ));		// 22
// 	indices.push_back( x    + xlen * (  y    + ylen * (z+1) ));		// 23
// 	indices.push_back((x+1) + xlen * (  y    + ylen * (z+1) ));		// 24
// 	indices.push_back((x-1) + xlen * ( (y+1) + ylen * (z+1) ));		// 25
// 	indices.push_back( x    + xlen * ( (y+1) + ylen * (z+1) ));		// 26
// 	indices.push_back((x+1) + xlen * ( (y+1) + ylen * (z+1) ));		// 27

// 	return indices;
// }

vector<Voxel> VoxelArray::surrounding_voxels(Vector3D xyz) {
	vector<Voxel> voxels;

	Voxel local = nearby_points(xyz);
	int x_idx = local.x_idx;
	int y_idx = local.y_idx;
	int z_idx = local.z_idx;

	for (int x = max(x_idx - 1, 0); x <= min(x_idx + 1, xlen - 1); ++x) {
		for (int y = max(y_idx - 1, 0); y <= min(y_idx + 1, ylen - 1); ++y) {
			for (int z = max(z_idx - 1, 0); z <= min(z_idx + 1, zlen - 1); ++z) {
				voxels.push_back(data[x][y][z]);
			}
		}
	}

	return voxels;
}

Sorter::Sorter(Vector3D xyz) : xyz(xyz) {};

bool Sorter::operator()(Vector3D a, Vector3D b) {
	return (a - xyz).norm2() > (b - xyz).norm2();
}

vector<Vector3D> VoxelArray::get_neighbors(Vector3D xyz) {
	vector<Voxel> voxels = surrounding_voxels(xyz);
	vector<Vector3D> neighbors;
	for (auto voxel : voxels) {
		for (auto point_idx : voxel.points) {
			neighbors.push_back(cloud[point_idx]);
		}
	}
	Sorter sorter(xyz);
	sort(neighbors.begin(), neighbors.end(), sorter);
	return neighbors;
}

}