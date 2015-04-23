/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef SHADOW_MAP_SHADER_H_
#define SHADOW_MAP_SHADER_H_
#include "Shader.h"

class ShadowMapShader : public Shader {
private:
	vmath::vec3 lightDir;	//VEC3
	GLuint ATM_RADIUS;
	
public:
	void init();

};

#endif