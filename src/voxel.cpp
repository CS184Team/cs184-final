#include "CGL/CGL.h"
#include <vector>

using namespace std;

namespace CGL {
	class VoxelArray {
	public:
		// Pass in the point cloud, bounding box low and high, and rho
		VoxelArray(std::vector<Vector3D> cloud, Vector3D low, Vector3D high, double rho);
		~VoxelArray();

		// Returns a list of point cloud indices which correspond to
		// the points contained within the voxel at xyz
		std::vector<int> nearby_points(Vector3D xyz);

		// Returns the voxel at voxel index
		std::vector<int> index(int index);

		// Returns a list of voxel indices, which correspond to the voxels surrounding
		// the point at xyz. Includes the local voxel
		std::vector<int> surrounding_voxel_indices(Vector3D xyz);

		// Returns a list of voxels (list of lists of point cloud indices),
		// which are the voxels surrounding the point at xyz.
		// Includes the local voxel
		std::vector<std::vector<int> > surrounding_voxels(Vector3D xyz);

	private:
		std::vector<std::vector<int> > data;
		double delta;
		int xlen;
		int ylen;
		int zlen;
	};

VoxelArray::VoxelArray(std::vector<Vector3D> cloud, Vector3D low, Vector3D high, double rho) {
	delta = 2.0 * rho;
	xlen = ceil((high.x - low.x) / delta);
	ylen = ceil((high.y - low.y) / delta);
	zlen = ceil((high.z - low.z) / delta);

	data.reserve(xlen * ylen * zlen);

	for (int i = 0; i < cloud.size(); i++) {
		int x = floor(cloud[i].x / delta);
		int y = floor(cloud[i].y / delta);
		int z = floor(cloud[i].z / delta);

		// http://stackoverflow.com/questions/20266201/3d-array-1d-flat-indexing
		data[ x + xlen * ( y + ylen * z ) ].push_back(i);
	}
}

VoxelArray::~VoxelArray() {
}

std::vector<int> VoxelArray::index(int index) {
	return data[index];
}

std::vector<int> VoxelArray::nearby_points(Vector3D xyz) {
	int x = floor(xyz.x / delta);
	int y = floor(xyz.y / delta);
	int z = floor(xyz.z / delta);
	return data[ x + xlen * ( y + ylen * z ) ];
}

std::vector<int> VoxelArray::surrounding_voxel_indices(Vector3D xyz) {
	int x = floor(xyz.x / delta);
	int y = floor(xyz.y / delta);
	int z = floor(xyz.z / delta);

	std::vector<int> indices;
	indices.push_back((x-1) + xlen * ( (y-1) + ylen * (z-1) ));		// 1
	indices.push_back( x    + xlen * ( (y-1) + ylen * (z-1) ));		// 2
	indices.push_back((x+1) + xlen * ( (y-1) + ylen * (z-1) ));		// 3
	indices.push_back((x-1) + xlen * (  y    + ylen * (z-1) ));		// 4
	indices.push_back( x    + xlen * (  y    + ylen * (z-1) ));		// 5
	indices.push_back((x+1) + xlen * (  y    + ylen * (z-1) ));		// 6
	indices.push_back((x-1) + xlen * ( (y+1) + ylen * (z-1) ));		// 7
	indices.push_back( x    + xlen * ( (y+1) + ylen * (z-1) ));		// 8
	indices.push_back((x+1) + xlen * ( (y+1) + ylen * (z-1) ));		// 9
	indices.push_back((x-1) + xlen * ( (y-1) + ylen *  z    ));		// 10
	indices.push_back( x    + xlen * ( (y-1) + ylen *  z    ));		// 11
	indices.push_back((x+1) + xlen * ( (y-1) + ylen *  z    ));		// 12
	indices.push_back((x-1) + xlen * (  y    + ylen *  z    ));		// 13
	indices.push_back( x    + xlen * (  y    + ylen *  z    ));		// 14
	indices.push_back((x+1) + xlen * (  y    + ylen *  z    ));		// 15
	indices.push_back((x-1) + xlen * ( (y+1) + ylen *  z    ));		// 16
	indices.push_back( x    + xlen * ( (y+1) + ylen *  z    ));		// 17
	indices.push_back((x+1) + xlen * ( (y+1) + ylen *  z    ));		// 18
	indices.push_back((x-1) + xlen * ( (y-1) + ylen * (z+1) ));		// 19
	indices.push_back( x    + xlen * ( (y-1) + ylen * (z+1) ));		// 20
	indices.push_back((x+1) + xlen * ( (y-1) + ylen * (z+1) ));		// 21
	indices.push_back((x-1) + xlen * (  y    + ylen * (z+1) ));		// 22
	indices.push_back( x    + xlen * (  y    + ylen * (z+1) ));		// 23
	indices.push_back((x+1) + xlen * (  y    + ylen * (z+1) ));		// 24
	indices.push_back((x-1) + xlen * ( (y+1) + ylen * (z+1) ));		// 25
	indices.push_back( x    + xlen * ( (y+1) + ylen * (z+1) ));		// 26
	indices.push_back((x+1) + xlen * ( (y+1) + ylen * (z+1) ));		// 27

	return indices;
}

std::vector<std::vector<int> > VoxelArray::surrounding_voxels(Vector3D xyz) {
	std::vector<int> indices = surrounding_voxel_indices(xyz);

	std::vector<std::vector<int> > voxels;

	for (int i = 0; i < 27; i++) {
		voxels.push_back(data[indices[i]]);
	}

	return voxels;
}

}