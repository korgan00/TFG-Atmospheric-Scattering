#include "Shader.h"


void Shader::init() {
	use();
	_commonUniforms.projectionMatrix = glGetUniformLocation(_renderProg, "projection_matrix");
	_commonUniforms.modelMatrix = glGetUniformLocation(_renderProg, "model_matrix");
	_commonUniforms.cam = glGetUniformLocation(_renderProg, "cam");

	CheckErr();
}

void Shader::preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
	use();
	projectionMatrix(projection_matrix);
	camera(vmath::vec3(cameraPos[0], cameraPos[1], cameraPos[2]));

}


void Shader::applyMaterial(Material* m, GLuint _tsoDiffuse, GLuint _tsoNormal){}