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
		GLuint C_EARTH;
		GLuint ATM_TOP_HEIGHT;
		GLuint ATM_RADIUS;
		GLuint ATM_RADIUS_2;
		GLuint PI;
		GLuint _3_16PI;
		GLuint _3_8PI;
		GLuint G;
		GLuint G2;
		GLuint P0;
		GLuint densityRayleigh;
		GLuint densityMie;
	} ScatteringUniformConstants_ids;

	typedef struct {
		GLfloat H_R;
		GLfloat H_M;
		GLfloat WORLD_RADIUS;
		vmath::vec3 C_EARTH;
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

	void init();

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