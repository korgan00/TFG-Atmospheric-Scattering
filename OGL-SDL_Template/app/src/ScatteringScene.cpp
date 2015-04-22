#include "ScatteringScene.h"


const GLfloat ScatteringScene::EARTH_RADIUS = 6360000.0f;
const GLfloat ScatteringScene::ATM_TOP_HEIGHT = 80000.0f;
const GLfloat ScatteringScene::DEEP_SPACE_RADIUS = EARTH_RADIUS * 1.4f;
const GLfloat ScatteringScene::MOUNTAINS_SCALE = 10.0f;

const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_RAYLEIGH = 7994.0f;
const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_MIE = 1200.0f;

void ScatteringScene::initOGLData() {

	ObjLoader::ObjFileInfo* mountainsObj = ObjLoader::load("Arid.obj");
	_mountains = ObjToMesh::convert(mountainsObj, new MountainTextureFactory());
	ObjLoader::ObjFileInfo* sphereObj = ObjLoader::load("sphere2.obj"); /*/ // Massive poligon
	ObjLoader::ObjFileInfo* sphereObj = ObjLoader::load("sphere.obj"); // light*/
	_blueSphere = ObjToMesh::convert(sphereObj);
	ObjLoader::ObjFileInfo* spaceObj = ObjLoader::load("deepSpace.obj");
	_deepSpace = ObjToMesh::convert(spaceObj);

	CheckErr();
	addMesh(_mountains);
	addMesh(_blueSphere);
	addMesh(_deepSpace);

	Scene::initOGLData();

	CheckErr();

	Shader::ScatteringUniformPseudoConstants_values spcValues;

	spcValues.lightDir = vmath::vec3(0.0f, 0.3f, 1.0f);
	spcValues.lightSun = 50.0f;

	spcValues.betaSR = vmath::vec3(5.8f, 13.5f, 33.1f) * 1e-6f;
	spcValues.betaSM = vmath::vec3(2.0f, 2.0f, 2.0f) * 1e-5f;
	spcValues.betaER = spcValues.betaSR;
	spcValues.betaEM = spcValues.betaSM * 1.1f;

	Shader::ScatteringUniformConstants_values scValues;

	scValues.H_R = 7994.0f;
	scValues.H_M = 1200.0f;
	scValues.WORLD_RADIUS = 6360000.0f;
	scValues.C_EARTH = vmath::vec3(0.0f, -scValues.WORLD_RADIUS, 0.0f);
	scValues.ATM_TOP_HEIGHT = 80000.0f;
	scValues.G = 0.76f;
	scValues.P0 = 1.0f;


	CheckErr();
	for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
		(*mesh)->scatteringVariables(spcValues);
		(*mesh)->scatteringConstants(scValues);
	}

	CheckErr();
	_mountains->modelMatrix(vmath::translate(0.0f, 0.0f, 0.0f) * 
							vmath::scale(MOUNTAINS_SCALE, MOUNTAINS_SCALE, MOUNTAINS_SCALE));
	_blueSphere->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * 
							 vmath::scale(EARTH_RADIUS, EARTH_RADIUS, EARTH_RADIUS));
	_deepSpace->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * 
							vmath::scale(DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS));

}