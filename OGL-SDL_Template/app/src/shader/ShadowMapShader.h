/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef SHADOW_MAP_SHADER_H_
#define SHADOW_MAP_SHADER_H_
#include "Shader.h"
#include <cmath>

class ShadowMapShader : public Shader {
private:
	int viewportSize;

	vmath::vec3 _lightDir;	//VEC3
	vmath::vec3 _cEarth;	//VEC3
	GLuint ATM_RADIUS;
	
	GLuint _framebufferName;
	vmath::mat4 _viewProjMatrix;

public:
	inline void lightDir(vmath::vec3 l) { _lightDir = l; }
	inline void cEarth(vmath::vec3 c) { _cEarth = c; }

	void init();

	inline vmath::mat4 viewProjectionMatrix() { return _viewProjMatrix; }

	void preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	static vmath::mat4 sunViewMatrix(vmath::vec3 lightDir, vmath::vec3 up, vmath::vec3 cEarth, float distance);

};

#endif