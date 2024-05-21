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

Matrix3d get_rotation_matrix(const Vector3d& v_before, const Vector3d& v_after)
{
	Vector3d vb = v_before / v_before.norm();
	Vector3d va = v_after / v_after.norm();

	Vector3d vs = vb.cross(va);
	Vector3d v = vs / vs.norm();
	double ca = vb.dot(va);

	double scale = 1 - ca;
	Vector3d vt{ v.x() * scale, v.y() * scale, v.z() * scale };

	Matrix3d rotationMatrix;

	rotationMatrix(0, 0) = vt.x() * v.x() + ca;
	rotationMatrix(1, 1) = vt.y() * v.y() + ca;
	rotationMatrix(2, 2) = vt.z() * v.z() + ca;
	vt.x() *= v.y();
	vt.z() *= v.x();
	vt.y() *= v.z();

	rotationMatrix(0, 1) = vt.x() - vs.z();
	rotationMatrix(0, 2) = vt.z() + vs.y();
	rotationMatrix(1, 0) = vt.x() + vs.z();
	rotationMatrix(1, 2) = vt.y() - vs.x();
	rotationMatrix(2, 0) = vt.z() - vs.y();
	rotationMatrix(2, 1) = vt.y() + vs.x();

	return rotationMatrix;
}

void get_sphere_mesh(ItcxDataReader& data, std::vector<Vector3d>& nodes, std::vector<Vector3i> &elements) {
	if (data.beams_.size() == 0)return;
	gmsh::initialize();
	gmsh::model::add("sphere");
	gmsh::model::occ::addSphere(0, 0, 0, 1);
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
	//
	int label = nodes.size();
	for (int i = 0; i < data.nodes_.size(); i++) {
		auto &node = data.nodes_[i];
		auto r = data.r_[i];
		for (int j = 0; j < sphere_mesh.nodes_.size(); j++) {
			nodes.push_back(sphere_mesh.nodes_[j] * r + node);
		}
		for (int j = 0; j < sphere_mesh.triangles_.size(); j++) {
			elements.push_back(sphere_mesh.triangles_[j].array() + label);
		}
		label += sphere_mesh.nodes_.size();
	}
}

void get_cylinder_mesh(ItcxDataReader& data, std::vector<Vector3d>& nodes, std::vector<Vector3i>& elements) {
	if (data.beams_.size() == 0)return;
	//x^2+y^2=1,z=0
	//生成一些点
	//std::vector<Vector3d> pts;
	gmsh::initialize();
	gmsh::model::add("cylinder");
	gmsh::model::occ::addCylinder(0, 0, 0, 0, 0, 1, data.r_.at(0));
	gmsh::model::occ::synchronize();
	gmsh::model::mesh::generate(2);
	CylinderMesh cylinder;
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
		int index = cylinder.add_point({ x,y,z });
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
		cylinder.add_triangles({ index1 ,index2 ,index3 });
	}

	int label = nodes.size();
	for (auto& beam : data.beams_) {
		auto id1 = beam[0];
		auto id2 = beam[1];
		auto &pt1 = data.nodes_.at(id1);
		auto& pt2 = data.nodes_.at(id2);
		auto d = (pt2 - pt1).norm();
		auto rm = get_rotation_matrix({ 0,0,1 }, pt2 - pt1);
		for (int j = 0; j < cylinder.nodes_.size(); j++) {
			Vector3d new_node = cylinder.nodes_[j];
			new_node(2) *= d;
			new_node = rm * new_node.eval();
			new_node += pt1;
			nodes.push_back(new_node);
		}
		for (int j = 0; j < cylinder.triangles_.size(); j++) {
			elements.push_back(cylinder.triangles_[j].array() + label);
		}
		label += cylinder.nodes_.size();
	}
}

void get_face_mesh(ItcxDataReader& data, std::vector<Vector3d>& nodes, std::vector<Vector3i>& elements) {
	for (auto& node : data.nodes_) {
		nodes.push_back(node);
	}
	for (auto& face : data.faces_) {
		if (face.size() == 3) {
			elements.push_back({ face[0] ,face[1] ,face[2] });
		}
		else {
			elements.push_back({ face[0] ,face[2] ,face[3] });
			elements.push_back({ face[0] ,face[1] ,face[2] });
		}
	}
}