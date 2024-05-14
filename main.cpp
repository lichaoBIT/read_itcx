#include "read_itcx.h"

int main() {
	string file = "D:/develop/ntop/exported_lattice3.ltcx";
	std::cout << "Read file : " << file << std::endl;
	ItcxDataReader data;
	data.read_itcx_data(file);
	return 0;
}