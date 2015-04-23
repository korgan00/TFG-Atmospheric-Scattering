#include "Shader.h"


void Shader::init() {
	_commonUniforms.projectionMatrix = glGetUniformLocation(_renderProg, "projection_matrix");
	_commonUniforms.modelMatrix = glGetUniformLocation(_renderProg, "model_matrix");
	_commonUniforms.cam = glGetUniformLocation(_renderProg, "cam");

	CheckErr();
}
