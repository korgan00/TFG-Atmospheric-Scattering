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
private:
	GLuint _renderProg;
public:

	void load(ShaderInfo* shaders) {
		_renderProg = LoadShaders(shaders);
	}

	void use(){
		glUseProgram(_renderProg);
	}

	void cleanup() {
		glUseProgram(0);
		glDeleteProgram(_renderProg);
	}

	int id() { return _renderProg; };
};

#endif