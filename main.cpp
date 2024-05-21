#include "read_itcx.h"
#include "mesh_lattice.h"

int main() {
	string name = "Honeycomb";
	string file = "D:/develop/ntop/" + name + ".ltcx";
	//string file = "D:/develop/ntop/IsoTruss.ltcx";
	std::cout << "Read file : " << file << std::endl;
	ItcxDataReader data;
	data.read_itcx_data(file); 
	std::vector<Vector3d> nodes; 
	std::vector<Vector3i> elements;
	get_sphere_mesh(data, nodes, elements);
	get_cylinder_mesh(data, nodes, elements);
	get_face_mesh(data, nodes, elements);
	write_stl_file("D:/develop/ntop/" + name + ".stl", nodes, elements);
	return 0;
}