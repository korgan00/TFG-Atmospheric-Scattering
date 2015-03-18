
#include "ObjLoader.h"

vector<Mesh> ObjLoader::load(string file) {

	vector<Mesh> shapes;
	GLuint accumulatedVertex = 1;

	int i = 0;

	// OPEN FILE
	ifstream input("../OGL-SDL_Template/app/resources/" + file);
	if (!input.is_open()) {
		cout << "Unable to open " + file + "\n";
		return shapes;
	}

	Mesh *m;
	while (m = Mesh::load(input, &accumulatedVertex)){
		shapes.push_back(*m);
	}
	
	// Close file
	input.close();

	return shapes;

}