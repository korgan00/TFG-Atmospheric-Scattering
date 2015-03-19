#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
#include "../common/common.h"
#include "SDL_image.h"
#include "Mesh.h"


class ObjLoader {
public:

	static vector<Mesh> loadMountains(string file);
	static Mesh* loadMountain(ifstream& input, GLuint* accVertex);
};

#endif /* OBJ_LOADER_H_ */