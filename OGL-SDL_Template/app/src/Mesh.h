/*
* Mesh.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef MESH_H_
#define MESH_H_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>
using namespace std;
#include "../common/common.h"
#include "SDL_image.h"

class Mesh {
private:

	typedef vmath::uvec3 Face;
	typedef vmath::vec3 Vertex;
	typedef vmath::vec2 TexVertex;

	Vertex *vertex;
	int vertexCount;
	GLuint sizeOfVertex;

	TexVertex *texCoord;
	int texCoordCount;
	GLuint sizeOfTexCoords;

	Face *faces;
	int facesCount;
	GLuint sizeOfFaces;

	std::string name;

	SDL_Surface* texture_A;
	GLuint texture_A_id;

	GLuint render_prog;
	GLuint render_model_matrix_loc;
	GLuint render_projection_matrix_loc;
	GLuint render_texture_loc;

	GLuint ebo[1];
	GLuint vao[1];
	GLuint vbo[1];


	void initializeSizes() {

		vertex = new Vertex[vertexCount];
		sizeOfVertex = vertexCount * sizeof(Vertex);

		texCoord = new TexVertex[texCoordCount];
		sizeOfTexCoords = texCoordCount * sizeof(TexVertex);
		
		faces = new Face[facesCount];
		sizeOfFaces = facesCount * sizeof(Face);

		cout << vertexCount << "::" << texCoordCount << "::" << facesCount << endl;
		cout << sizeOfVertex << ", " << sizeOfTexCoords << ", " << sizeOfFaces << endl;
	}

public:

	Mesh();

	static Mesh* load(ifstream& input, GLuint* accumulatedVertex);

	void draw(vmath::mat4 model_matrix, vmath::mat4 projection_matrix);

	void initOGLData();


};


#endif /* MESH_H_ */
