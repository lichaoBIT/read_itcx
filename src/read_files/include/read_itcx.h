#pragma once
//#define eigen_assert(x)
//	do {
//	if (!(x)) throw std::runtime_error(#x);
//	}while(false)

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <Eigen/Eigen>
using string = std::string;
using Vector3d = Eigen::Vector3d;
using Vector3i = Eigen::Vector3i;
using Matrix3d = Eigen::Matrix3d;

class ItcxDataReader {
public:
	std::vector<double> r_;
	std::vector<Vector3d> nodes_;
	std::vector<std::array<int, 2>> beams_;
	std::vector<std::vector<int>> faces_;
	void read_itcx_data(const string &file);
};

void write_stl_file(const string& path, const std::vector<Vector3d>& nodes,
	const std::vector<Vector3i>& triangles);