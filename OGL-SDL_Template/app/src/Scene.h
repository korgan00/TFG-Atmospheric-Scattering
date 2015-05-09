

#ifndef SCENE_H_
#define SCENE_H_
#include <vector>
#include "../common/common.h"
#include "mesh/Mesh.h"
#include "shader/Shader.h"

class Scene {
private:
	void initPostProcessPanel();
protected:
	std::vector<Mesh*> _sceneObjects;
	Shader* _activeShader;

	GLuint _postProVAO;
	GLuint _postProVBO;
	GLuint _postProEBO;
public:
	Scene() : _sceneObjects(std::vector<Mesh*>()) {}

	//GETTER
	std::vector<Mesh*> sceneObjects() { return _sceneObjects; }
	//SETTER
	void sceneObjects(std::vector<Mesh*> sceneObjs) { _sceneObjects = sceneObjs; }
	//MODIFIERS
	void addMesh(std::vector<Mesh*> sceneObjs) {
		for (std::vector<Mesh*>::iterator mesh = sceneObjs.begin(); mesh != sceneObjs.end(); ++mesh) {
			_sceneObjects.push_back(*mesh);
		}
	}
	void addMesh (Mesh* mesh) { _sceneObjects.push_back(mesh); }

	//ACTIONS
	void initOGLData() {
		for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
			(*mesh)->initOGLData();
		}
		initPostProcessPanel();
		CheckErr();
	}


	void cleanup() {
		for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
			(*mesh)->cleanup();
		}

		CheckErr();
	}

	void draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos);

	void postProcessDraw();

};

#endif /* SCENE_H_ */