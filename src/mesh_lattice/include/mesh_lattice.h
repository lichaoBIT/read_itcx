#pragma once
#include "read_itcx.h"

class SphereMesh {
public:
	std::vector<Vector3d> nodes_;
	std::vector<Vector3i> triangles;
};