

#ifndef SCENE_H_
#define SCENE_H_
#include <vector>
#include "../common/common.h"
#include "mesh/Mesh.h"
#include "Shader.h"

class Scene {
private:
	std::vector<Mesh> _sceneObjects;

	Shader _shader;
	GLuint _render_projection_matrix_loc;
	GLuint _render_texture_loc;

	//scattering uniforms
	GLuint _uniform_lightDir; //VEC3
	GLuint _uniform_cam; //VEC3
	GLuint _uniform_density; //FLOAT[][]
	GLuint _uniform_betaER; //VEC3
	GLuint _uniform_betaEM; //VEC3
	GLuint _uniform_betaSR; //VEC3
	GLuint _uniform_betaSM; //VEC3
	GLuint _uniform_lightSun; //FLOAT

public:
	Scene() : _sceneObjects(std::vector<Mesh>()), _shader(),
		_render_projection_matrix_loc(0), _render_texture_loc(0), _uniform_lightDir(0),
		_uniform_cam(0), _uniform_density(0), _uniform_betaER(0), _uniform_betaEM(0),
		_uniform_betaSR(0), _uniform_betaSM(0), _uniform_lightSun(0) {}

	std::vector<Mesh> sceneObjects() { return _sceneObjects; }
	void sceneObjects(std::vector<Mesh> sceneObjs) { _sceneObjects = sceneObjs; }
	void addObjects(std::vector<Mesh> sceneObjs) {
		for (std::vector<Mesh>::iterator mesh = sceneObjs.begin(); mesh != sceneObjs.end(); ++mesh) {
			_sceneObjects.push_back(*mesh);
		}
	}
	void addObject(Mesh mesh) {	_sceneObjects.push_back(mesh); }

	void initOGLData() {

		// Seleccionamos los shaders que queremos cargar
		ShaderInfo shaderFiles[] = {
			{ GL_VERTEX_SHADER, "../OGL-SDL_Template/app/shaders/scattering.vs.glsl" },
			{ GL_FRAGMENT_SHADER, "../OGL-SDL_Template/app/shaders/scattering.fs.glsl" },
			{ GL_NONE, NULL }
		};

		_shader.load(shaderFiles);
		_shader.use();
		_render_texture_loc = glGetUniformLocation(_shader.id(), "texture_diffuse");
		glUniform1i(_render_texture_loc, 0); //Texture unit 0 is for base images.
		_render_projection_matrix_loc = glGetUniformLocation(_shader.id(), "projection_matrix");


		_uniform_lightDir = glGetUniformLocation(_shader.id(), "lightDir");
		_uniform_cam = glGetUniformLocation(_shader.id(), "cam");
		//_uniform_density = glGetUniformLocation(_render_prog, "density");
		_uniform_betaER = glGetUniformLocation(_shader.id(), "betaER");
		_uniform_betaEM = glGetUniformLocation(_shader.id(), "betaEM");
		_uniform_betaSR = glGetUniformLocation(_shader.id(), "betaSR");
		_uniform_betaSM = glGetUniformLocation(_shader.id(), "betaSM");
		_uniform_lightSun = glGetUniformLocation(_shader.id(), "lightSun");

		for (std::vector<Mesh>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
			//mesh->initOGLData();
		}
	}


	void cleanup() {
		_shader.cleanup();

		for (std::vector<Mesh>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
			//mesh->cleanup();
		}
	}

	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
		_shader.use();

		// Posicion en el shader, cantidad de matrices, es traspuesta? , matriz a setear.
		// Guardamos las dos matrices en el shader para que dibuje.
		// Al ser uniform el valor, se aprovecha entre los shaders
		glUniformMatrix4fv(_render_projection_matrix_loc, 1, GL_FALSE, projection_matrix);

		vmath::vec3 lightDir = vmath::vec3(0.0f, -1.0f, 1.0f);
		vmath::vec3 cam = vmath::vec3(cameraPos[0], cameraPos[1], cameraPos[2]);
		// usamos la unidad de openGL como "un metro"
		// se pude escalar esto correspondientemente para que actue 
		// como si cada unidad de OGL fueran varios metros
		vmath::vec3 betaSR = vmath::vec3(5.8f, 13.5f, 33.1f) * 10e-6f;
		vmath::vec3 betaSM = vmath::vec3(2.0f, 2.0f, 2.0f) * 10e-5f;

		vmath::vec3 betaER = betaSR;
		vmath::vec3 betaEM = betaSM * 1.1f;

		glUniform3fv(_uniform_lightDir, 1, projection_matrix);
		glUniform3fv(_uniform_cam, 1, cam);
		//glUniform3fv(_uniform_density, 1, projection_matrix);
		glUniform3fv(_uniform_betaER, 1, betaER);
		glUniform3fv(_uniform_betaEM, 1, betaEM);
		glUniform3fv(_uniform_betaSR, 1, betaSR);
		glUniform3fv(_uniform_betaSM, 1, betaSM);
		glUniform1f(_uniform_lightSun, 20.0f);

		for (std::vector<Mesh>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
			//mesh->draw(projection_matrix, cameraPos);
		}
	}
};

#endif /* SCENE_H_ */