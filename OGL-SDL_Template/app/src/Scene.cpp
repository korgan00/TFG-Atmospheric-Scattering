#include "Scene.h"


void Scene::draw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
	/*_activeShader->use();
	_activeShader->projectionMatrix(projection_matrix);
	_activeShader->camera(vmath::vec3(cameraPos[0], cameraPos[1], cameraPos[2]));
	*/
	_activeShader->preDraw(projection_matrix, cameraPos);
	CheckErr();

	for (std::vector<Mesh*>::iterator mesh = _sceneObjects.begin(); mesh != _sceneObjects.end(); ++mesh) {
		_activeShader->modelMatrix((*mesh)->modelMatrix());
		(*mesh)->draw(_activeShader);
	}
	CheckErr();
}

void Scene::initPostProcessPanel() {

	glGenVertexArrays(1, &_postProVAO);
	glBindVertexArray(_postProVAO);

	struct VertexData {
		GLfloat position[2];
		GLfloat textureCoord[2];
	};

	VertexData vertices[4] = {
		{ { -1.0f, -1.0f }, { 0.00f, 0.00f } },
		{ { -1.0f, 1.0f }, { 0.00f, 1.00f } },
		{ { 1.0f, -1.0f }, { 1.00f, 0.00f } },
		{ { 1.0f, 1.0f }, { 1.00f, 1.00f } }
	};

	GLushort eboData[4] = { 0, 2, 1, 3 };

	glGenBuffers(1, &_postProVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _postProVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(VertexData), (const GLvoid*)sizeof(vertices[0].position));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &_postProEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _postProEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eboData), &eboData[0], GL_STATIC_DRAW);

}

void Scene::postProcessDraw() {
	_activeShader->preDraw(vmath::mat4(), vmath::vec4());
	glBindVertexArray(_postProVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _postProEBO);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, NULL);
}