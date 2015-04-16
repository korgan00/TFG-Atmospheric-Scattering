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

	Shader _shader;

	SDL_Surface* _texture;
	//GLuint _uniformProjectionId;
	GLuint *_ebo;
	GLuint *_tso;
	GLuint _vao[1];
	GLuint _vbo[1];
	
public:

	Mesh(PerVertex* verticesInfo, GLuint verticesCount, PerDraw* drawInfo, GLuint drawCount) :
		_vertexBufferObjectData(verticesInfo), _vboDataCount(verticesCount), 
		_elementBufferObjectData(drawInfo), _eboDataCount(drawCount), _shader() {}

	// SETTERS
	void modelMatrix(vmath::mat4 modelMatrix);
	void scatteringVariables(Shader::ScatteringUniformPseudoConstants_values scattValues);
	void scatteringConstants(Shader::ScatteringUniformConstants_values scattValues);

	//ACTIONS
	void initOGLData();
	void cleanup();
	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);
};


#endif /* MESH_H_ */
