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

#include "../../common/common.h"

class Shader {
public:
	typedef struct {
		GLuint projectionMatrix;
		GLuint modelMatrix;
		GLuint cam;			//VEC3
	} CommonUniforms;
protected:
	GLuint _renderProg;
	CommonUniforms _commonUniforms;
public:

	void load(ShaderInfo* shaders) {
		_renderProg = LoadShaders(shaders);
	}

	virtual void init();

	virtual void preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	inline void use(){ glUseProgram(_renderProg); }

	inline GLuint id() { return _renderProg; };

	inline void cleanup() {
		glUseProgram(0);
		glDeleteProgram(_renderProg);
	}

	inline void projectionMatrix(vmath::mat4 projMat) {
		glUniformMatrix4fv(_commonUniforms.projectionMatrix, 1, GL_FALSE, projMat);
	}

	inline void modelMatrix(vmath::mat4 modelMat) {
		glUniformMatrix4fv(_commonUniforms.modelMatrix, 1, GL_FALSE, modelMat);
	}

	inline void camera(vmath::vec3 cam) {
		glUniform3fv(_commonUniforms.cam, 1, cam);
	}
};

#endif