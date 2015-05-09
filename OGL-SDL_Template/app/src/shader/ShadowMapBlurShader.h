/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef SHADOW_MAP_BLUR_SHADER_H_
#define SHADOW_MAP_BLUR_SHADER_H_
#include "Shader.h"
#include <cmath>

class ShadowMapBlurShader : public Shader {
private:
	int viewportSize;

	GLuint _framebufferName;
	GLuint _renderbufferName;
public:
	void init();

	void preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	void applyMaterial(Material* m, GLuint _tsoDiffuse, GLuint _tsoNormal);
};

#endif