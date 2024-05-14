#include "read_itcx.h"
#include <sstream>

void ItcxDataReader::read_itcx_data(const string& file) {
	std::ifstream ifs{ file };
	string str;
	std::vector<string> lines;
	while (getline(ifs, str)) {
		lines.emplace_back(str);
	}
	ifs.close();
	auto nlines = (int)lines.size();
	for (int i = 0; i < nlines; i++) {
		const auto& line = lines[i];
		if (line.find("node id") != string::npos) {
			int pos = 0;
			std::vector<int> positions;
			while ((pos = line.find("\"", pos)) != string::npos) {
				positions.push_back(pos+1);
				pos++;
			}
			auto substr1 = line.substr(positions[2], positions[3] - positions[2] - 1);
			auto substr2 = line.substr(positions[4], positions[5] - positions[4] - 1);
			auto substr3 = line.substr(positions[6], positions[7] - positions[6] - 1);
			
			//std::array<double, 3> coords{ std::stod(substr1) ,std::stod(substr2) ,std::stod(substr3) };
			nodes_.push_back({ std::stod(substr1), std::stod(substr2), std::stod(substr3) });
			if (positions.size() > 8) {
				auto substr4 = line.substr(positions[8], positions[9] - positions[8] - 1);
				r_.push_back(std::stod(substr4));
			}
		}
		else if (line.find("beam id") != string::npos) {
			int pos = 0;
			std::vector<int> positions;
			while ((pos = line.find("\"", pos)) != string::npos) {
				positions.push_back(pos + 1);
				pos++;
			}
			auto substr1 = line.substr(positions[2], positions[3] - positions[2] - 1);
			auto substr2 = line.substr(positions[4], positions[5] - positions[4] - 1);
			beams_.push_back({ std::stoi(substr1), std::stoi(substr2)});
		}
		else if (line.find("face id") != string::npos) {
			int pos = 0;
			std::vector<int> positions;
			while ((pos = line.find("\"", pos)) != string::npos) {
				positions.push_back(pos + 1);
				pos++;
			}
			int nlabels = positions.size() / 2;
			std::vector<int> face;
			for (int j = 1; j < nlabels; j++) {
				auto substr1 = line.substr(positions[2*j], positions[2 * j+1] - positions[2 * j] - 1);
				face.push_back(std::stoi(substr1));
			}
			faces_.push_back(std::move(face));
		}
		else { ; }
	}
}