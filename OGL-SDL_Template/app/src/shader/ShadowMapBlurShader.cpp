#include "ShadowMapBlurShader.h"

void ShadowMapBlurShader::init() {

	Shader::init();
	viewportSize = 4096;

	GLuint tex = glGetUniformLocation(_renderProg, "texture_diffuse");
	glUniform1i(tex, 7);

	CheckErr();

	glGenFramebuffers(1, &_framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferName);

	CheckErr();

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportSize, viewportSize,
		0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	CheckErr();

	
	glGenRenderbuffers(1, &_renderbufferName);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderbufferName);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, viewportSize, viewportSize);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthTexture, 0);

	// attach a renderbuffer to depth attachment point
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderbufferName);


	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	CheckErr();

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::error("CHECKING FRAMEBUFFER STATUS");
		return;
	}

}


void ShadowMapBlurShader::applyMaterial(Material* m, GLuint _tsoDiffuse, GLuint _tsoNormal) {}

void ShadowMapBlurShader::preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
	use();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebufferName);
	glViewport(0, 0, viewportSize, viewportSize);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CheckErr();
}
