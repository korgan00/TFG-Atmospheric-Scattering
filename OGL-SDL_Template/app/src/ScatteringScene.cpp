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
	ObjLoader::ObjFileInfo* sphereObj = ObjLoader::load("sphere.obj");
	_blueSphere = ObjToMesh::convert(sphereObj);
	ObjLoader::ObjFileInfo* spaceObj = ObjLoader::load("deepSpace.obj");
	_deepSpace = ObjToMesh::convert(spaceObj);

	CheckErr();
	addMesh(_mountains);
	addMesh(_blueSphere);
	addMesh(_deepSpace);

	Scene::initOGLData();

	CheckErr();

	Shader::ScatteringUniformPseudoConstants_values spcValues = {
		vmath::vec3(0.0f, -1.0f, 0.1f), // LIGHT_DIR
		20.0f, // LIGHT_SUN
		vmath::vec3(5.8f, 13.5f, 33.1f) * 10e-6f, // betaSR
		vmath::vec3(2.0f, 2.0f, 2.0f) * 10e-5f, // betaSM
		vmath::vec3(5.8f, 13.5f, 33.1f) * 10e-6f, // betaER
		vmath::vec3(2.0f, 2.0f, 2.0f) * 10e-5f * 1.1f, // betaEM
	};
	Shader::ScatteringUniformConstants_values scValues = {
		7994.0f, //GLfloat H_R;
		1200.0f, //GLfloat H_M;
		6360000.0f,// * 10e6f, //GLfloat WORLD_RADIUS;
		vmath::vec3(0.0f, -6360000.0f, 0.0f),//GLfloat C_EARTH;
		80000.0f,// * 10e4f, //GLfloat ATM_TOP_HEIGHT;
		0.76f, //GLfloat G;
		1.0f //GLfloat P0;
	};

	CheckErr();
	for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
		(*mesh)->scatteringVariables(spcValues);
		(*mesh)->scatteringConstants(scValues);
	}

	CheckErr();
	_mountains->modelMatrix(vmath::translate(0.0f, 0.0f, 0.0f) * vmath::scale(MOUNTAINS_SCALE, MOUNTAINS_SCALE, MOUNTAINS_SCALE));
	_blueSphere->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * vmath::scale(EARTH_RADIUS, EARTH_RADIUS, EARTH_RADIUS));
	_deepSpace->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) * vmath::scale(DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS, DEEP_SPACE_RADIUS));

}