/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef SHADER_H_
#define SHADER_H_
#include <iostream>
#include <math.h>
#include "SDL_image.h"

#include "../common/common.h"

class Shader {
public:
	typedef struct {
		GLuint projectionMatrix;
		GLuint modelMatrix;
		GLuint cam;			//VEC3
	} CommonUniforms;

	typedef struct {
		GLuint lightDir;	//VEC3
		GLuint lightSun;	//FLOAT

		//GLuint cam;			//VEC3
		//GLuint density;		//FLOAT[][]

		GLuint betaER;		//VEC3
		GLuint betaEM;		//VEC3
		GLuint betaSR;		//VEC3
		GLuint betaSM;		//VEC3
	} ScatteringUniformPseudoConstants_ids;

	typedef struct {
		vmath::vec3 lightDir;	//VEC3
		GLfloat lightSun;	//FLOAT

		//GLuint density;		//FLOAT[][]

		vmath::vec3 betaER;		//VEC3
		vmath::vec3 betaEM;		//VEC3
		vmath::vec3 betaSR;		//VEC3
		vmath::vec3 betaSM;		//VEC3
	} ScatteringUniformPseudoConstants_values;

	typedef struct {
		GLuint H_R;
		GLuint H_M;
		GLuint WORLD_RADIUS;
		GLuint ATM_TOP_HEIGHT;
		GLuint ATM_RADIUS;
		GLuint ATM_RADIUS_2;
		GLuint PI;
		GLuint _3_16PI;
		GLuint _3_8PI;
		GLuint G;
		GLuint G2;
		GLuint P0;
	} ScatteringUniformConstants_ids;

	typedef struct {
		GLfloat H_R;
		GLfloat H_M;
		GLfloat WORLD_RADIUS;
		GLfloat ATM_TOP_HEIGHT;
		GLfloat G;
		GLfloat P0;
	} ScatteringUniformConstants_values;

private:
	GLuint _renderProg;
	CommonUniforms _commonUniforms;
	ScatteringUniformPseudoConstants_ids _SUids;
	ScatteringUniformConstants_ids _SUconst;
public:
	GLuint _tso[2];

	void load(ShaderInfo* shaders) {
		_renderProg = LoadShaders(shaders);
	}

	void init() {
		_commonUniforms.projectionMatrix = glGetUniformLocation(_renderProg, "projection_matrix");
		_commonUniforms.modelMatrix = glGetUniformLocation(_renderProg, "model_matrix");
		_commonUniforms.cam = glGetUniformLocation(_renderProg, "cam");


		_SUids.lightDir = glGetUniformLocation(_renderProg, "lightDir");
		_SUids.lightSun = glGetUniformLocation(_renderProg, "lightSun");
		_SUids.betaER = glGetUniformLocation(_renderProg, "betaER");
		_SUids.betaEM = glGetUniformLocation(_renderProg, "betaEM");
		_SUids.betaSR = glGetUniformLocation(_renderProg, "betaSR");
		_SUids.betaSM = glGetUniformLocation(_renderProg, "betaSM");


		_SUconst.H_R = glGetUniformLocation(_renderProg, "H_R");
		_SUconst.H_M = glGetUniformLocation(_renderProg, "H_M");
		_SUconst.WORLD_RADIUS = glGetUniformLocation(_renderProg, "WORLD_RADIUS");
		_SUconst.ATM_TOP_HEIGHT = glGetUniformLocation(_renderProg, "ATM_TOP_HEIGHT");
		_SUconst.ATM_RADIUS = glGetUniformLocation(_renderProg, "ATM_RADIUS");
		_SUconst.ATM_RADIUS_2 = glGetUniformLocation(_renderProg, "ATM_RADIUS_2");
		_SUconst.PI = glGetUniformLocation(_renderProg, "M_PI");
		_SUconst._3_16PI = glGetUniformLocation(_renderProg, "_3_16PI");
		_SUconst._3_8PI = glGetUniformLocation(_renderProg, "_3_8PI");
		_SUconst.G = glGetUniformLocation(_renderProg, "G");
		_SUconst.G2 = glGetUniformLocation(_renderProg, "G2");
		_SUconst.P0 = glGetUniformLocation(_renderProg, "P0");

		glGenTextures(2, _tso);

	}

	void use(){
		glUseProgram(_renderProg);
	}

	void cleanup() {
		glUseProgram(0);
		glDeleteProgram(_renderProg);
	}

	GLuint id() { return _renderProg; };

	void projectionMatrix(vmath::mat4 projMat) {
		glUniformMatrix4fv(_commonUniforms.projectionMatrix, 1, GL_FALSE, projMat);
	}

	void modelMatrix(vmath::mat4 modelMat) {
		glUniformMatrix4fv(_commonUniforms.modelMatrix, 1, GL_FALSE, modelMat);
	}

	void camera(vmath::vec3 cam) {
		glUniform3fv(_commonUniforms.cam, 1, cam);
	}

	void scatteringVariables(ScatteringUniformPseudoConstants_values scattValues);

	void scatteringConstants(ScatteringUniformConstants_values scattValues);

	void createHeightScatterMap(ScatteringUniformConstants_values scattValues,
		SDL_Surface* &texR, SDL_Surface* &texM);
};

#endif