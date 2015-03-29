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
#include "Shader.h"

class Mesh {
public:
	typedef vmath::uvec3 Face;
	typedef vmath::vec3 Vertex;
	typedef vmath::vec2 TexVertex;

private:

	Vertex *_vertices;
	GLint _vertexCount;
	GLuint _sizeOfVertex;

	TexVertex *_texCoord;
	GLint _texCoordCount;
	GLuint _sizeOfTexCoords;

	Face *_faces;
	GLint _facesCount;
	GLuint _sizeOfFaces;

	std::string _name;

	SDL_Surface* _texture;
	GLuint _texture_id;

	Shader _shader;
	GLuint _render_projection_matrix_loc;
	GLuint _render_texture_loc;

	//scattering uniforms
	GLuint _uniform_lightDir; //VEC3
	GLuint _uniform_cam; //VEC3
	GLuint _uniform_density; //FLOAT[][]
	GLuint _uniform_betaER; //VEC3
	GLuint _uniform_betaEM; //VEC3
	GLuint _uniform_betaSR; //VEC3
	GLuint _uniform_betaSM; //VEC3
	GLuint _uniform_lightSun; //FLOAT

	GLuint _ebo[1];
	GLuint _vao[1];
	GLuint _vbo[1];

public:

	Mesh();

	void initializeSizes(GLint vertCount, GLint texCount, GLint facesCount) {
		_vertexCount = vertCount;
		_vertices = new Vertex[_vertexCount];
		_sizeOfVertex = _vertexCount * sizeof(Vertex);

		_texCoordCount = texCount;
		_texCoord = new TexVertex[_texCoordCount];
		_sizeOfTexCoords = _texCoordCount * sizeof(TexVertex);

		_facesCount = facesCount;
		_faces = new Face[facesCount];
		_sizeOfFaces = facesCount * sizeof(Face);

		cout << _vertexCount << "::" << _texCoordCount << "::" << _facesCount << endl;
		cout << _sizeOfVertex << ", " << _sizeOfTexCoords << ", " << _sizeOfFaces << endl;
	}

	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	void initOGLData();
	void cleanup();

	inline GLint vertexCount() { return _vertexCount; }
	inline Vertex* vertices() { return _vertices; }

	inline GLint texCoordCount() { return _texCoordCount; }
	inline TexVertex* texCoord() { return _texCoord; }

	inline GLint facesCount() { return _facesCount; }
	inline Face* faces() { return _faces; }

	inline void name(string n) { _name = n; }
	inline string name() { return _name; }

};


#endif /* MESH_H_ */
