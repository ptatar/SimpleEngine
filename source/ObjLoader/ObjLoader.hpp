#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace ObjLoader {

struct ObjData {
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> coords;
	std::vector<unsigned> indices;

	std::vector<std::string> materials;
};


ObjData LoadObjFile(std::string name) {
	std::ifstream fs;
	fs.open(name);
	if (!fs.is_open()) {
		throw FileNotFound(name);
	}
}

}