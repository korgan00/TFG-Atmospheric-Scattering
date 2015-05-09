
#ifndef SCATT_SCENE_H_
#define SCATT_SCENE_H_
#include "Scene.h"
#include "mesh/ObjLoader.h"
#include "mesh/ObjToMesh.h"
#include "mesh/MountainTextureFactory.h"
#include "shader/ScatteringShader.h"
#include "shader/ShadowMapShader.h"
#include "shader/ShadowMapBlurShader.h"

class ScatteringScene : public Scene {
private:

	static const GLfloat   EARTH_RADIUS;
	static const GLfloat   ATM_TOP_HEIGHT;
	static const GLfloat   DEEP_SPACE_RADIUS;
	static const GLfloat   MOUNTAINS_SCALE;

	static const GLfloat   PARTICLE_SCALE_HEIGHT_RAYLEIGH;
	static const GLfloat   PARTICLE_SCALE_HEIGHT_MIE;

	Mesh* _mountains;
	Mesh* _blueSphere;
	Mesh* _deepSpace;
	//Mesh* _sun;

	ScatteringShader* _scatteringShading;
	ShadowMapShader* _shadowMapShading;
	ShadowMapBlurShader* _shadowMapBlurShading;

	ScatteringShader::ScatteringUniformPseudoConstants_values scattPseudoConstValues();
	ScatteringShader::ScatteringUniformConstants_values scattConstValues();

public:
	void initOGLData();

	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);
};

#endif /* SCATT_SCENE_H_ */