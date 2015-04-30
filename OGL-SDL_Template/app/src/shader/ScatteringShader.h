/*
* CameraFPS.h
*
*  Created on: 16/03/2015
*      Author: Korgan
*/

#ifndef SCATTERING_SHADER_H_
#define SCATTERING_SHADER_H_
#include "Shader.h"
#include "ShadowMapShader.h"

class ScatteringShader : public Shader {
public:
	typedef struct {
		GLuint lightDir;	//VEC3
		GLuint lightSun;	//FLOAT

		GLuint betaER;		//VEC3
		GLuint betaEM;		//VEC3
		GLuint betaSR;		//VEC3
		GLuint betaSM;		//VEC3
		GLuint depthBiasVP;
	} ScatteringUniformPseudoConstants_ids;

	typedef struct {
		vmath::vec3 lightDir;	//VEC3
		GLfloat lightSun;	//FLOAT

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
		GLuint shadowMap;
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
	ScatteringUniformPseudoConstants_ids _SUids;
	ScatteringUniformConstants_ids _SUconst;

	static SDL_Surface *texDensityRay, *texDensityMie;

	void createHeightScatterMap(ScatteringUniformConstants_values scattValues,
		SDL_Surface* &texR, SDL_Surface* &texM);

	bool intersection(vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 &t1, 
		vmath::vec3 &t2, vmath::vec3 cEarth, float atmRadius_2);

	GLuint _tso[2];
public:
	ShadowMapShader* _shadowMapShader;

	void init();

	void preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	void scatteringVariables(ScatteringUniformPseudoConstants_values scattValues);

	void scatteringConstants(ScatteringUniformConstants_values scattValues);
};

#endif