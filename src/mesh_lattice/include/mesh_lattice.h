#pragma once
#include "read_itcx.h"

class SphereMesh {
public:
	std::vector<Vector3d> nodes_;
	std::vector<Vector3i> triangles_;
	int add_point(const Vector3d &coord);
	void add_triangles(const Vector3i& labels);
};

class CylinderMesh {
public:
	std::vector<Vector3d> nodes_;
	std::vector<Vector3i> triangles;
};

void get_sphere_mesh(ItcxDataReader &data, std::vector<Vector3d> &nodes, std::vector<Vector3i>& elements);