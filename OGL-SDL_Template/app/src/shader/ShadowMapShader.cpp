#include "ShadowMapShader.h"

void ShadowMapShader::init() {

	Shader::init();
	viewportSize = 1024;

	glGenFramebuffers(1, &_framebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferName);

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, viewportSize, viewportSize,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	/*
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	*/
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::error("CHECKING FRAMEBUFFER STATUS");
		return;
	}

}

void ShadowMapShader::preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {
	vmath::vec3 camPosSimple(cameraPos[0], cameraPos[1], cameraPos[2]);
	Shader::preDraw(
		sunViewMatrix(vmath::normalize(_lightDir),
		vmath::vec3(0.0f, 1.0f, 0.001f),
		camPosSimple,
		10000.0f), cameraPos);

	glViewport(0, 0, viewportSize, viewportSize);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebufferName);
	glBlitFramebuffer(0, 0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT,
						0, 0, viewportSize,		 viewportSize,
						GL_DEPTH_BUFFER_BIT,	 GL_NEAREST);
	CheckErr();

}

vmath::mat4 ShadowMapShader::sunViewMatrix(vmath::vec3 lightDir, vmath::vec3 up, 
		vmath::vec3 cEarth, float distance) {
	vmath::mat4 ortho = vmath::mat4::identity();
	GLfloat halfWidth = 10000.0f;
	GLfloat halfHeight = 10000.0f;
	GLfloat zNear = 0.0f;
	GLfloat zFar = 20000.0f;

	ortho[0][0] = 1 / halfWidth;// 100.0f / right;
	ortho[1][1] = 1 / halfHeight;// 100.0f / top;
	ortho[2][2] = -2.0f / (zFar - zNear);
	ortho[3][2] = -(zFar + zNear) / (zFar - zNear);
	ortho[3][3] = 1.0f;

	vmath::vec3 a(0.0f, 0.0f, -1.0f);
	vmath::vec3 v = vmath::cross(a, lightDir);
	float s = vmath::length(v);
	float c = vmath::dot(a, lightDir);

	vmath::mat4 vx({ vmath::vec4(0.0f, -v[2], v[1], 0.0f),
		vmath::vec4(v[2], 0.0f, -v[0], 0.0f),
		vmath::vec4(-v[1], v[0], 0.0f, 0.0f),
		vmath::vec4(0.0f, 0.0f, 0.0f, 0.0f) });

	float aux = ((1 - c) / (s*s));
	vmath::mat4 vx2 = (vx*vx);
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
			vx2[x][y] *= aux;

	vmath::mat4 r = vmath::mat4::identity() + vx + vx2;
	vmath::matNM<float, 4, 1> m = (0.0f, 0.0f, 0.0f, 1.0f);

	vmath::mat4 suntrans = vmath::translate(cEarth) * vmath::translate(-lightDir * distance);
	suntrans = suntrans.transpose();
	vmath::vec4 sunPos(0.0f, 0.0f, 0.0f, 1.0f);

	for (int x = 0; x < 4; x++)	sunPos[x] = vmath::dot(suntrans[x], sunPos);

	vmath::vec3 sunPosSimple(sunPos[0], sunPos[1], sunPos[2]);
	vmath::mat4 lookAt;
	vmath::vec3 X, Y, Z;

	Z = sunPosSimple - cEarth;
	Z = vmath::normalize(Z);
	Y = up;
	X = vmath::cross(Y, Z);

	Y = vmath::cross(Z, X);

	X = vmath::normalize(X);
	Y = vmath::normalize(Y);

	lookAt[0][0] = X[0];
	lookAt[1][0] = X[1];
	lookAt[2][0] = X[2];
	lookAt[3][0] = vmath::dot(-X, -lightDir);
	lookAt[0][1] = Y[0];
	lookAt[1][1] = Y[1];
	lookAt[2][1] = Y[2];
	lookAt[3][1] = vmath::dot(-Y, -lightDir);
	lookAt[0][2] = Z[0];
	lookAt[1][2] = Z[1];
	lookAt[2][2] = Z[2];
	lookAt[3][2] = vmath::dot(-Z, -lightDir);
	lookAt[0][3] = 0.0f;
	lookAt[1][3] = 0.0f;
	lookAt[2][3] = 0.0f;
	lookAt[3][3] = 1.0f;


	//GLfloat aspect = float(DEFAULT_WIN_HEIGHT) / float(DEFAULT_WIN_WIDTH);
	ortho = ortho *
		lookAt * vmath::translate(-sunPosSimple);// * r;
	// -5000.0f, -8000.0f, -5000.0f);

	return ortho;
}