#include "ScatteringScene.h"


const GLfloat ScatteringScene::EARTH_RADIUS = 6360000.0f;
const GLfloat ScatteringScene::ATM_TOP_HEIGHT = 80000.0f;
const GLfloat ScatteringScene::DEEP_SPACE_RADIUS = EARTH_RADIUS * 2.0f;
const GLfloat ScatteringScene::MOUNTAINS_SCALE = 10.0f;

const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_RAYLEIGH = 7994.0f;
const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_MIE = 1200.0f;

void ScatteringScene::initOGLData() {

	_sunRotation = new vmath::vec2(0.0f, 0.0f);
	_sunRotating = new vmath::vec2(0.0f, 0.0f);
	_updateShadowMap = true;

	ShaderInfo scatteringFiles[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/scattering.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/scattering.fs.glsl" },
		{ GL_NONE, NULL }
	};

	ShaderInfo orthoFiles[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/shadowMap.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/shadowMap.fs.glsl" },
		{ GL_NONE, NULL }
	};

	ScatteringShader::ScatteringUniformPseudoConstants_values sPCV = scattPseudoConstValues();
	ScatteringShader::ScatteringUniformConstants_values sCV = scattConstValues();

	_scatteringShading = new ScatteringShader();
	_shadowMapShading = new ShadowMapShader();

	_scatteringShading->load(scatteringFiles);
	_scatteringShading->use();
	_scatteringShading->init();
	_scatteringShading->scatteringVariables(sPCV);
	_scatteringShading->scatteringConstants(sCV);

	CheckErr();


	_shadowMapShading->load(orthoFiles);
	_shadowMapShading->lightDir(sPCV.lightDir);
	_shadowMapShading->cEarth(sCV.C_EARTH);
	_shadowMapShading->use();
	_shadowMapShading->init();

	_scatteringShading->_shadowMapShader = _shadowMapShading;
	
	CheckErr();

	_activeShader = _shadowMapShading;

	//_mountains = ObjToMesh::convert(ObjLoader::load("mtn pass.obj"));
	//_mountains = ObjToMesh::convert(ObjLoader::load("The City.obj"));
	//_mountains = ObjToMesh::convert(ObjLoader::load("Mountain Valley.obj"));
	//_mountains = ObjToMesh::convert(ObjLoader::load("majestic mountain.obj"));
	_mountains = ObjToMesh::convert(ObjLoader::load("AridOpti.obj"), new MountainTextureFactory());
	//_mountains = ObjToMesh::convert(ObjLoader::load("Arid.obj"), new MountainTextureFactory());
	_blueSphere = ObjToMesh::convert(ObjLoader::load("sphere2.obj")); /*/ // Massive poligon
	_blueSphere = ObjToMesh::convert(ObjLoader::load("sphere.obj")); // light*/
	_deepSpace  = ObjToMesh::convert(ObjLoader::load("deepSpace.obj"));
	//_sun		= ObjToMesh::convert(ObjLoader::load("sun.obj"));
	
	addMesh(_mountains);
	addMesh(_deepSpace);
	addMesh(_blueSphere);

	Scene::initOGLData();

	_mountains->modelMatrix(
		vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::scale(MOUNTAINS_SCALE, MOUNTAINS_SCALE, MOUNTAINS_SCALE));
	_blueSphere->modelMatrix(
		vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) *
		vmath::scale(EARTH_RADIUS, EARTH_RADIUS, EARTH_RADIUS));
	_deepSpace->modelMatrix(
		vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) *
		vmath::scale(DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS));

	CheckErr();
}

ScatteringShader::ScatteringUniformConstants_values ScatteringScene::scattConstValues() {
	ScatteringShader::ScatteringUniformConstants_values scValues;

	scValues.H_R = 7994.0f;
	scValues.H_M = 1200.0f;
	scValues.WORLD_RADIUS = 6360000.0f;
	scValues.C_EARTH = vmath::vec3(0.0f, -scValues.WORLD_RADIUS, 0.0f);
	scValues.ATM_TOP_HEIGHT = 80000.0f;
	scValues.G = 0.76f;
	scValues.P0 = 1.0f;

	return scValues;
}

ScatteringShader::ScatteringUniformPseudoConstants_values ScatteringScene::scattPseudoConstValues() {
	ScatteringShader::ScatteringUniformPseudoConstants_values spcValues;

	vmath::vec4 sunRot = (vmath::rotate((*_sunRotation)[0], vmath::vec3(1.0f, 0.0f, 0.0f)) * vmath::translate(0.0f, -1.0f, 0.0f))[3];

	spcValues.lightDir = vmath::vec3(sunRot[0], sunRot[1], sunRot[2]);
	spcValues.lightSun = 50.0f;

	spcValues.betaSR = vmath::vec3(5.8f, 13.5f, 33.1f) * 1e-6f;
	spcValues.betaSM = vmath::vec3(2.0f, 2.0f, 2.0f) * 1e-5f;
	spcValues.betaER = spcValues.betaSR;
	spcValues.betaEM = spcValues.betaSM * 1.1f;

	return spcValues;
}

void ScatteringScene::draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {

	if (_updateShadowMap) {
		_activeShader = _shadowMapShading;
		_deepSpace->visible(false);
		Scene::draw(projection_matrix, cameraPos);
		_updateShadowMap = false;
	}
	
	_activeShader = _scatteringShading;
	_deepSpace->visible(true);
	Scene::draw(projection_matrix, cameraPos);
}

void ScatteringScene::tick(GLfloat time, GLfloat elapsedTime) {
	if ((*_sunRotating)[0] != 0.0f || (*_sunRotating)[1] != 0.0f) {
		*_sunRotation += *_sunRotating * (elapsedTime / 60);

		ScatteringShader::ScatteringUniformPseudoConstants_values sPCV = scattPseudoConstValues();
		_scatteringShading->scatteringVariables(sPCV);
		_shadowMapShading->lightDir(sPCV.lightDir);
		_updateShadowMap = true;
	}
}

void ScatteringScene::rotateSunX(GLfloat amount) {
	(*_sunRotating)[0] = amount;
}

bool ScatteringScene::sunIsRotating() {
	return (*_sunRotating)[0] != 0.0f;
}