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
#include "../../common/common.h"
#include "SDL_image.h"
#include "../Shader.h"
using namespace std;

class Mesh {
public:
	typedef struct {
		Vertex v;
		Normal vn;
		TexVertex vt;
	} PerVertex;

	typedef struct {
		Material *mat;
		GLuint *indices;
		GLuint indicesCount;
		GLuint indicesSize;
	} PerDraw;

private:
	PerVertex *_vertexBufferObjectData;
	GLuint _vboDataCount;
	GLuint _vboDataSize;
	PerDraw *_elementBufferObjectData;
	GLuint _eboDataCount;

	SDL_Surface* _texture;
	GLuint *_ebo;
	GLuint *_tso;
	GLuint _vao[1];
	GLuint _vbo[1];
	
	vmath::mat4 _modelMatrix;

public:

	//Shader _shader;

	Mesh(PerVertex* verticesInfo, GLuint verticesCount, PerDraw* drawInfo, GLuint drawCount) :
		_vertexBufferObjectData(verticesInfo), _vboDataCount(verticesCount), 
		_elementBufferObjectData(drawInfo), _eboDataCount(drawCount) {}

	// SETTERS
	void modelMatrix(vmath::mat4 modelMatrix) { _modelMatrix = modelMatrix; }
	vmath::mat4 modelMatrix() { return _modelMatrix; }

	//ACTIONS
	void initOGLData();
	void cleanup();
	void draw();
};


#endif /* MESH_H_ */
