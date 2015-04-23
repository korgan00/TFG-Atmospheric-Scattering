#include "ScatteringScene.h"


const GLfloat ScatteringScene::EARTH_RADIUS = 6360000.0f;
const GLfloat ScatteringScene::ATM_TOP_HEIGHT = 80000.0f;
const GLfloat ScatteringScene::DEEP_SPACE_RADIUS = EARTH_RADIUS * 1.4f;
const GLfloat ScatteringScene::MOUNTAINS_SCALE = 10.0f;

const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_RAYLEIGH = 7994.0f;
const GLfloat ScatteringScene::PARTICLE_SCALE_HEIGHT_MIE = 1200.0f;

void ScatteringScene::initOGLData() {

	ShaderInfo scatteringFiles[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/scattering.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/scattering.fs.glsl" },
		//{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/gouraud.frag" },
		{ GL_NONE, NULL }
	};

	ShaderInfo orthoFiles[] = {
		{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/scattering.vs.glsl" },
		{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/scattering.fs.glsl" },
		//{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/gouraud.frag" },
		{ GL_NONE, NULL }
	};

	_scatteringShading = new ScatteringShader();
	//_orthoShading = new Shader();

	_scatteringShading->load(scatteringFiles);
	//_orthoShading->load(orthoFiles);
	/*
	_orthoShading->use();
	_orthoShading->init();
	_orthoShading->modelMatrix(vmath::mat4::identity());
	*/

	_activeShader = _scatteringShading;
	_activeShader->use();

	_activeShader->init();
	_activeShader->modelMatrix(vmath::mat4::identity());

	CheckErr();

	_scatteringShading->scatteringVariables(scattPseudoConstValues());
	_scatteringShading->scatteringConstants(scattConstValues());

	CheckErr();

	_mountains  = ObjToMesh::convert(ObjLoader::load("Arid.obj"), new MountainTextureFactory());
	_blueSphere = ObjToMesh::convert(ObjLoader::load("sphere2.obj")); /*/ // Massive poligon
	_blueSphere = ObjToMesh::convert(ObjLoader::load("sphere.obj")); // light*/
	_deepSpace  = ObjToMesh::convert(ObjLoader::load("deepSpace.obj"));
	//_sun		= ObjToMesh::convert(ObjLoader::load("sun.obj"));

	addMesh(_mountains);
	addMesh(_blueSphere);
	addMesh(_deepSpace);
	//addMesh(_sun);

	Scene::initOGLData();

	_mountains->modelMatrix(vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::scale(MOUNTAINS_SCALE, MOUNTAINS_SCALE, MOUNTAINS_SCALE));
	_blueSphere->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) *
		vmath::scale(EARTH_RADIUS, EARTH_RADIUS, EARTH_RADIUS));
	_deepSpace->modelMatrix(vmath::translate(0.0f, -EARTH_RADIUS, 0.0f) *
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

	spcValues.lightDir = vmath::vec3(0.0f, -1.0f, 1.0f);
	spcValues.lightSun = 50.0f;

	spcValues.betaSR = vmath::vec3(5.8f, 13.5f, 33.1f) * 1e-6f;
	spcValues.betaSM = vmath::vec3(2.0f, 2.0f, 2.0f) * 1e-5f;
	spcValues.betaER = spcValues.betaSR;
	spcValues.betaEM = spcValues.betaSM * 1.1f;

	return spcValues;
}

void ScatteringScene::draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
	_activeShader = _shadowMapShading;
	//cameraPos = loquesea;
	Scene::draw(projection_matrix, cameraPos);
	//bufferCopy
	_activeShader = _scatteringShading;
	Scene::draw(projection_matrix, cameraPos);
}