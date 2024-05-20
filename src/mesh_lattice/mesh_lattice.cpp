#include "mesh_lattice.h"
#include "gmsh.h"
#include <map>

string get_coord_key(const Vector3d& coord) {
	/*auto a1 = boost::format("%.6f") % x;
	auto a2 = boost::format("%.6f") % y;
	auto a3 = boost::format("%.6f") % z;
	x = std::atof(a1.str().data());
	y = std::atof(a2.str().data());
	z = std::atof(a3.str().data());
	std::string key;
	key = "." + std::to_string(x) + "." + std::to_string(y) \
		+ "." + std::to_string(z);
	return key;*/
	return "";
}

int SphereMesh::add_point(const Vector3d& coord) {
	//auto key = get_coord_key(coord);
	auto label = nodes_.size();
	nodes_.emplace_back(coord);
	return label;
}

void SphereMesh::add_triangles(const Vector3i& labels) {
	triangles_.emplace_back(labels);
}

void get_sphere_mesh(ItcxDataReader& data, std::vector<Vector3d>& nodes, std::vector<Vector3i> &elements) {
	//auto &nodes = data.nodes_;
	//auto& radius = data.r_;
	gmsh::initialize();
	gmsh::model::add("sphere");
	gmsh::model::occ::addSphere(0, 0, 0, 2.5);
	gmsh::model::occ::synchronize();
	gmsh::model::mesh::generate(2);

	SphereMesh sphere_mesh;

	//gmsh::model::mesh::getNode
	//gmsh::write("D:/develop/metamaterial/TEST/sphere.stl");
	int typ = 2; //3 - node triangles
	std::vector<std::size_t> elementTags;
	std::vector<std::size_t> nodeTags1;
	gmsh::model::mesh::getElementsByType(typ, elementTags, nodeTags1);

	std::vector<std::size_t> nodeTags2;
	std::vector<double> coord;
	std::vector<double> parametricCoord;
	gmsh::model::mesh::getNodes(nodeTags2, coord, parametricCoord);

	std::vector<std::size_t> nodeTags3;
	gmsh::model::mesh::getElementEdgeNodes(typ, nodeTags3);
	gmsh::finalize();

	//注意gmsh从1开始编号
	//存入点、点坐标并去重——gmsh已经完成去重，但编号顺序错开——保留位数4
	if (coord.size() % nodeTags2.size() != 0) {
		std::cout << "coord error" << std::endl;
		throw;
	}
	std::map<int, int> map;
	for (int k = 0; k < nodeTags2.size(); k++) {
		auto nid = nodeTags2[k] - 1;//-1
		auto x = coord[3 * k];
		auto y = coord[3 * k + 1];
		auto z = coord[3 * k + 2];
		int index = sphere_mesh.add_point({ x,y,z });
		map.insert({ nid,index });
	}
	//存入三角面片并去重——测试gmsh是否完成去重——保留位数
	if (nodeTags1.size() % elementTags.size() != 0) {
		std::cout << "triangle error" << std::endl;
		throw;
	}
	for (int k = 0; k < elementTags.size(); k++) {
		auto tid = elementTags[k];
		auto l1 = nodeTags1[3 * k] - 1;
		auto l2 = nodeTags1[3 * k + 1] - 1;
		auto l3 = nodeTags1[3 * k + 2] - 1;
		auto index1 = map.at(l1);
		auto index2 = map.at(l2);
		auto index3 = map.at(l3);
		sphere_mesh.add_triangles({ index1 ,index2 ,index3 });
	}
	write_stl_file("D:/develop/ntop/exported_lattice3.stl", sphere_mesh.nodes_, sphere_mesh.triangles_);
}