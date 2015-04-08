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

#include "../common/common.h"

class Shader {
public:
	typedef struct {
		GLuint projectionMatrix;
		GLuint modelMatrix;
	} CommonUniforms;

	typedef struct {
		GLuint lightDir;	//VEC3
		GLuint lightSun;	//FLOAT

		GLuint cam;			//VEC3
		GLuint density;		//FLOAT[][]

		GLuint betaER;		//VEC3
		GLuint betaEM;		//VEC3
		GLuint betaSR;		//VEC3
		GLuint betaSM;		//VEC3
	} ScatteringUniforms;
private:
	GLuint _renderProg;
	CommonUniforms _commonUniforms;
	ScatteringUniforms _scatteringUniforms;
public:

	void load(ShaderInfo* shaders) {
		_renderProg = LoadShaders(shaders);
	}

	void init() {
		_commonUniforms.projectionMatrix = glGetUniformLocation(_renderProg, "projection_matrix");
		_commonUniforms.modelMatrix = glGetUniformLocation(_renderProg, "model_matrix");


		_scatteringUniforms.lightDir = glGetUniformLocation(_renderProg, "lightDir");
		_scatteringUniforms.lightSun = glGetUniformLocation(_renderProg, "lightSun");

		_scatteringUniforms.cam = glGetUniformLocation(_renderProg, "cam");
		//_scatteringUniforms.density = glGetUniformLocation(_renderProg, "density");

		_scatteringUniforms.betaER = glGetUniformLocation(_renderProg, "betaER");
		_scatteringUniforms.betaEM = glGetUniformLocation(_renderProg, "betaEM");
		_scatteringUniforms.betaSR = glGetUniformLocation(_renderProg, "betaSR");
		_scatteringUniforms.betaSM = glGetUniformLocation(_renderProg, "betaSM");
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
		glUniformMatrix4fv(_commonUniforms.modelMatrix , 1, GL_FALSE, modelMat);
	}

	void camera(vmath::vec3 cam) {
		glUniform3fv(_scatteringUniforms.cam, 1, cam);
		//glUniformMatrix4fv(_scatteringUniforms.density, 1, GL_FALSE, *density);
	}

	void scatteringVariables(vmath::vec3 lightDir, GLfloat lightSun, vmath::vec3 betaER, 
			vmath::vec3 betaEM, vmath::vec3 betaSR, vmath::vec3 betaSM) {
		//glUniformMatrix4fv(_scatteringUniforms.density, 1, GL_FALSE, *density);

		glUniform3fv(_scatteringUniforms.lightDir, 1, lightDir);
		glUniform1f(_scatteringUniforms.lightSun, lightSun);

		glUniform3fv(_scatteringUniforms.betaER, 1, betaER);
		glUniform3fv(_scatteringUniforms.betaEM, 1, betaEM);
		glUniform3fv(_scatteringUniforms.betaSR, 1, betaSR);
		glUniform3fv(_scatteringUniforms.betaSM, 1, betaSM);
	}

};

#endif