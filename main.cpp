#include "read_itcx.h"
#include "mesh_lattice.h"

int main() {
	string file = "D:/develop/ntop/exported_lattice3.ltcx";
	std::cout << "Read file : " << file << std::endl;
	ItcxDataReader data;
	data.read_itcx_data(file); 
	std::vector<Vector3d> nodes; 
	std::vector<Vector3i> elements;
	get_sphere_mesh(data, nodes, elements);
	/*write_stl_file("D:/develop/ntop/exported_lattice3.stl", nodes, elements);*/
	/*{
		std::ofstream ofs;
		ofs.open("D:/develop/metamaterial/TEST/node.txt", std::ios::out);
		for(auto &node: data.nodes_){
			ofs << node(0) << " " << node(1) << " " << node(2) << std::endl;
		}
		ofs.close();
	}
	{
		std::ofstream ofs;
		ofs.open("D:/develop/metamaterial/TEST/beams.txt", std::ios::out);
		for (auto& beam : data.beams_) {
			ofs << beam[0] << " " << beam[1] << std::endl;
		}
		ofs.close();
	}
	{
		std::ofstream ofs;
		ofs.open("D:/develop/metamaterial/TEST/r.txt", std::ios::out);
		for (auto r : data.r_) {
			ofs << r << std::endl;
		}
		ofs.close();
	}*/

	return 0;
}