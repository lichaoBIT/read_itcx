#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <Eigen/Core>
using string = std::string;
using Vector3d = Eigen::Vector3d;
using Vector3i = Eigen::Vector3i;

class ItcxDataReader {
public:
	std::vector<double> r_;
	std::vector<Vector3d> nodes_;
	std::vector<std::array<int, 2>> beams_;
	std::vector<std::vector<int>> faces_;
	void read_itcx_data(const string &file);
};