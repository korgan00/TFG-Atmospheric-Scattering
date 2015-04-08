#include "ScatteringScene.h"


const GLfloat ScatteringScene::EARTH_RADIUS = 6360000.0f;
const GLfloat ScatteringScene::ATM_TOP_HEIGHT = 80000.0f;
const GLfloat ScatteringScene::DEEP_SPACE_RADIUS = EARTH_RADIUS * 1.2f;
const GLfloat ScatteringScene::MOUNTAINS_SCALE = 20.0f;

const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_RAYLEIGH = 7994.0f;
const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_MIE = 1200.0f;

void ScatteringScene::initOGLData() {

	ObjLoader::ObjFileInfo* mountainsObj = ObjLoader::load("Arid.obj");
	_mountains = ObjToMesh::convert(mountainsObj, new MountainTextureFactory());
	ObjLoader::ObjFileInfo* sphereObj = ObjLoader::load("waterWorld.obj");
	_blueSphere = ObjToMesh::convert(sphereObj);
	ObjLoader::ObjFileInfo* spaceObj = ObjLoader::load("deepSpace.obj");
	_deepSpace = ObjToMesh::convert(spaceObj);

	CheckErr();
	addMesh(_mountains);
	addMesh(_blueSphere);
	addMesh(_deepSpace);

	Scene::initOGLData();

	CheckErr();
	vmath::vec3 lightDir = vmath::vec3(0.0f, -1.0f, 0.1f);
	GLfloat lightSun = 30.0f;
	// usamos la unidad de openGL como "un metro"
	// se pude escalar esto correspondientemente para que actue 
	// como si cada unidad de OGL fueran varios metros
	vmath::vec3 betaSR = vmath::vec3(5.8f, 13.5f, 33.1f) * 10e-6f;
	vmath::vec3 betaSM = vmath::vec3(2.0f, 2.0f, 2.0f) * 10e-5f;

	vmath::vec3 betaER = betaSR;
	vmath::vec3 betaEM = betaSM * 1.1f;

	CheckErr();
	for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
		(*mesh)->scatteringVariables(lightDir, lightSun, betaER, betaEM, betaSR, betaSM);
	}

	CheckErr();
	_deepSpace->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * vmath::scale(DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS));
	_blueSphere->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * vmath::scale(EARTH_RADIUS, EARTH_RADIUS, EARTH_RADIUS));
	_mountains->modelMatrix(vmath::translate(0.0f, 0.0f, 0.0f) * vmath::scale(MOUNTAINS_SCALE, MOUNTAINS_SCALE, MOUNTAINS_SCALE));

}