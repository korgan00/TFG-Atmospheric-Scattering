
#include "ObjLoader.h"

vector<Mesh> ObjLoader::loadMountains(string file) {
	/* Posiciones interesantes: 
	 * -25.2987, 7.7, 229.612 
	 * 316.775, 4.65, -229.822
	 * Altura máxima aprox: 42-45.
	 */


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
	while (m = ObjLoader::loadMountain(input, &accumulatedVertex)) {
		shapes.push_back(*m);
	}
	
	// Close file
	input.close();

	return shapes;

}


Mesh* ObjLoader::loadMountain(ifstream& input, GLuint* accVertex) {
	//ifstream input = *in;
	streamoff firstLine = input.tellg();

	Mesh* m = new Mesh();

	string line;
	bool gRead = false;

	GLint vertexCount = 0;
	GLint texCoordCount = 0;
	GLint facesCount = 0;

	while (getline(input, line) && !(line[0] == 'v' && gRead)) {
		switch (line[0]) {
		case 'v':
			switch (line[1]) {
			case ' ': vertexCount++; break;
			case 't': texCoordCount++; break;
			default:;
			}
			break;
		case 'g': gRead = true;		break;
		case 'f': facesCount++;		break;
		default:;
		}
	}

	if (vertexCount == 0) return NULL;

	input.clear();
	input.seekg(firstLine);

	m->initializeSizes(vertexCount, texCoordCount, facesCount);

	/******* POPULATING ARRAYS *******/
	GLuint accumulatedVertex = *accVertex;
	*accVertex += m->vertexCount();


	int currentVertex = 0;
	int currentTextCoord = 0;
	int currentFace = 0;

	float x, y, z;
	GLuint xFace, yFace, zFace;
	char auxCString[100];

	while (currentFace < facesCount) {
		getline(input, line);

		switch (line[0]) {
		case 'v': // Vertices & textures
			sscanf(line.c_str(), "%s %f %f %f", &auxCString, &x, &y, &z);
			switch (line[1]) {
			case ' ':
				m->vertices()[currentVertex] = Mesh::Vertex(x, y, z);
				currentVertex++;
				break;
			case 't':
				m->texCoord()[currentTextCoord] = Mesh::TexVertex(x, y);
				currentTextCoord++;
				break;
			default:;
			}
			break;
		case 'g': // Group
			sscanf(line.c_str(), "g %s", &auxCString);
			m->name(string(auxCString));
			cout << m->name() << endl; //Mesh_Arid_{A,B, ... , J}_XX de aqui se puede sacar la textura
			break;
		case 'f': // Faces (Triangles)
			sscanf(line.c_str(), "f %f/%u %f/%u %f/%u", &x, &xFace, &y, &yFace, &z, &zFace);
			m->faces()[currentFace] = Mesh::Face(xFace - accumulatedVertex, yFace - accumulatedVertex, zFace - accumulatedVertex);
			currentFace++;
			break;
		default:;
		}
	}

	return m;
}
