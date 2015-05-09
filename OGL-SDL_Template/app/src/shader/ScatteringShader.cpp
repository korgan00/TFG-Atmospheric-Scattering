#include "ScatteringShader.h"

SDL_Surface* ScatteringShader::texDensityRay = nullptr;
SDL_Surface* ScatteringShader::texDensityMie = nullptr;


void ScatteringShader::init() {

	Shader::init();

	_SUids.lightDir = glGetUniformLocation(_renderProg, "lightDir");
	_SUids.lightSun = glGetUniformLocation(_renderProg, "lightSun");
	_SUids.betaER = glGetUniformLocation(_renderProg, "betaER");
	_SUids.betaEM = glGetUniformLocation(_renderProg, "betaEM");
	_SUids.betaSR = glGetUniformLocation(_renderProg, "betaSR");
	_SUids.betaSM = glGetUniformLocation(_renderProg, "betaSM");
	_SUids.depthBiasVP = glGetUniformLocation(_renderProg, "depthBiasVP");
	CheckErr();


	_SUconst.H_R = glGetUniformLocation(_renderProg, "H_R");
	_SUconst.H_M = glGetUniformLocation(_renderProg, "H_M");
	_SUconst.WORLD_RADIUS = glGetUniformLocation(_renderProg, "WORLD_RADIUS");
	_SUconst.C_EARTH = glGetUniformLocation(_renderProg, "C_EARTH");
	_SUconst.ATM_TOP_HEIGHT = glGetUniformLocation(_renderProg, "ATM_TOP_HEIGHT");
	_SUconst.ATM_RADIUS = glGetUniformLocation(_renderProg, "ATM_RADIUS");
	_SUconst.ATM_RADIUS_2 = glGetUniformLocation(_renderProg, "ATM_RADIUS_2");
	_SUconst.PI = glGetUniformLocation(_renderProg, "M_PI");
	_SUconst._3_16PI = glGetUniformLocation(_renderProg, "_3_16PI");
	_SUconst._3_8PI = glGetUniformLocation(_renderProg, "_3_8PI");
	_SUconst.G = glGetUniformLocation(_renderProg, "G");
	_SUconst.G2 = glGetUniformLocation(_renderProg, "G2");
	_SUconst.P0 = glGetUniformLocation(_renderProg, "P0");

	_textureIds.diffuse = glGetUniformLocation(_renderProg, "texture_diffuse");
	_textureIds.normal = glGetUniformLocation(_renderProg, "texture_normalmap");

	CheckErr();


	_textureIds.densityRayleigh = glGetUniformLocation(_renderProg, "densityRayleigh");
	_textureIds.densityMie = glGetUniformLocation(_renderProg, "densityMie");
	_textureIds.shadowMap = glGetUniformLocation(_renderProg, "shadowMap");

	glGenTextures(2, _tso);
	CheckErr();

}

void ScatteringShader::scatteringVariables(ScatteringUniformPseudoConstants_values scattValues) {
	glUniform3fv(_SUids.lightDir, 1, scattValues.lightDir);
	glUniform1f(_SUids.lightSun, scattValues.lightSun);
	CheckErr();

	glUniform3fv(_SUids.betaER, 1, scattValues.betaER);
	glUniform3fv(_SUids.betaEM, 1, scattValues.betaEM);
	glUniform3fv(_SUids.betaSR, 1, scattValues.betaSR);
	glUniform3fv(_SUids.betaSM, 1, scattValues.betaSM);
	CheckErr();
}

void ScatteringShader::scatteringConstants(ScatteringUniformConstants_values scattValues) {
	glUniform1f(_SUconst.H_R, scattValues.H_R);
	glUniform1f(_SUconst.H_M, scattValues.H_M);
	glUniform1f(_SUconst.WORLD_RADIUS, scattValues.WORLD_RADIUS);
	glUniform3fv(_SUconst.C_EARTH, 1, scattValues.C_EARTH);
	glUniform1f(_SUconst.ATM_TOP_HEIGHT, scattValues.ATM_TOP_HEIGHT);
	GLfloat atmRad = scattValues.ATM_TOP_HEIGHT + scattValues.WORLD_RADIUS;
	glUniform1f(_SUconst.ATM_RADIUS, atmRad);
	glUniform1f(_SUconst.ATM_RADIUS_2, atmRad*atmRad);
	glUniform1f(_SUconst.PI, (float)M_PI);
	glUniform1f(_SUconst._3_16PI, 3.0f / (16.0f * (float)M_PI));
	glUniform1f(_SUconst._3_8PI, 3.0f / (8.0f * (float)M_PI));
	glUniform1f(_SUconst.G, scattValues.G);
	glUniform1f(_SUconst.G2, scattValues.G * scattValues.G);
	glUniform1f(_SUconst.P0, scattValues.P0);

	glUniform1i(_textureIds.diffuse, 0);
	glUniform1i(_textureIds.normal, 2);

	glUniform1i(_textureIds.densityRayleigh, 6);
	glUniform1i(_textureIds.densityMie, 5);
	glUniform1i(_textureIds.shadowMap, 8);
	
	CheckErr();


	if (texDensityRay == nullptr || texDensityMie == nullptr) {
		createHeightScatterMap(scattValues, texDensityRay, texDensityMie);
	}

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, _tso[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texDensityRay->w, texDensityRay->h, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, texDensityRay->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	CheckErr();


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _tso[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texDensityMie->w, texDensityMie->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texDensityMie->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	CheckErr();

	//IMG_SavePNG(texM, "C:/Users/MisiKorgan/Desktop/pruebaScatterMapMie.png");
	//IMG_SavePNG(texR, "C:/Users/MisiKorgan/Desktop/pruebaScatterMapRay.png");
}

void ScatteringShader::applyMaterial(Material* mat, GLuint _tsoDiffuse, GLuint _tsoNormal) {
	glActiveTexture(GL_TEXTURE0);
	if (mat != nullptr && mat->textureDiffuse != "") {
		glBindTexture(GL_TEXTURE_2D, _tsoDiffuse);
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glActiveTexture(GL_TEXTURE2);
	if (mat != nullptr && mat->textureNormalMap != "") {
		glBindTexture(GL_TEXTURE_2D, _tsoNormal);
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


void ScatteringShader::preDraw(vmath::mat4 projection_matrix, vmath::vec4 cameraPos) {

	Shader::preDraw(projection_matrix, cameraPos);

	vmath::mat4 biasMatrix( vmath::vec4(0.5f, 0.f,  0.f,  0.f),
							vmath::vec4(0.f,  0.5f, 0.f,  0.f),
							vmath::vec4(0.f,  0.f,  0.5f, 0.f),
							vmath::vec4(0.5f, 0.5f, 0.5f, 1.f));


	glUniformMatrix4fv(_SUids.depthBiasVP, 1, GL_FALSE, biasMatrix * _shadowMapShader->viewProjectionMatrix());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, DEFAULT_WIN_WIDTH, DEFAULT_WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void ScatteringShader::createHeightScatterMap(ScatteringUniformConstants_values scattValues, SDL_Surface* &texR, SDL_Surface* &texM) {

	texR = IMG_Load("../OGL-SDL_Template/app/resources/ObjTex/white.png");
	texM = IMG_Load("../OGL-SDL_Template/app/resources/ObjTex/white2.png");

	GLfloat atmHeight = scattValues.ATM_TOP_HEIGHT;
	GLfloat atmRadius = scattValues.WORLD_RADIUS + scattValues.ATM_TOP_HEIGHT;
	GLfloat atmRadius2 = atmRadius * atmRadius;
	GLfloat earthRadius = scattValues.WORLD_RADIUS;
	GLfloat H_R = scattValues.H_R;
	GLfloat H_M = scattValues.H_M;
	GLfloat P0 = scattValues.P0;

	GLfloat STEPS = 50.0f;

	Uint32 *pixR = (Uint32 *)texR->pixels;
	Uint32 *pixM = (Uint32 *)texM->pixels;

	for (GLint w = 0; w < texR->w; w += 1) {
		GLfloat initialHeight = ((atmRadius - earthRadius) * w) / texR->w;
		GLfloat point_earth = initialHeight + earthRadius;
		vmath::vec3 point(0.0f, point_earth, 0.0f);

		for (GLint h = 0; h < texR->h; h += 1) {
			GLfloat density_AP[2] = { 0.0, 0.0 };

			GLfloat cosPhi = (2.0f * h / (texR->h - 1)) - 1.0f;
			GLfloat senPhi = sqrt(1.0f - cosPhi*cosPhi);
			vmath::vec3 normLightDir(senPhi, -cosPhi, 0.0f);

			vmath::vec3 t1, t2;
			bool intersects = intersection(point, point + (-normLightDir * 15000000.0f),
				t1, t2, vmath::vec3(0.0f, 0.0f, 0.0f), atmRadius2);

			if (!intersects) {
				stringstream ss("NOT INTERSECTING: ");
				ss << "h:" << initialHeight << " ; cosPhi: " << cosPhi;
				Log::warning(ss.str());
			}

			GLfloat diferential_A = vmath::distance(t1, t2) / STEPS;
			vmath::vec3 delta_A(-normLightDir * diferential_A);

			for (GLfloat step = 0.5f; step < STEPS; step += 1.0f) {
				GLfloat hPoint = vmath::length(point + delta_A * step) - earthRadius;

				GLfloat relation[2] = { -hPoint / H_R, -hPoint / H_M };
				density_AP[0] += P0 * exp(relation[0]) * diferential_A;
				density_AP[1] += P0 * exp(relation[1]) * diferential_A;
			}

			pixR[(h * texR->w) + w] = (Uint32)density_AP[0];
			pixM[(h * texR->w) + w] = (Uint32)density_AP[1];
		}
	}
}



bool ScatteringShader::intersection(vmath::vec3 p1, vmath::vec3 p2, vmath::vec3 &t1, vmath::vec3 &t2, vmath::vec3 cEarth, float atmRadius_2) {
	vmath::vec3 rayD = vmath::normalize(p2 - p1);
	vmath::vec3 oc = p1 - cEarth;

	float b = 2.0f * vmath::dot(rayD, oc);
	float c = vmath::dot(oc, oc) - atmRadius_2;
	float disc = b*b - 4.0f*c;

	t1 = p1;
	t2 = p2;

	if (disc < 0.0f) return false;

	float d0 = (-b - sqrtf(disc)) / 2.0f;
	float d1 = (-b + sqrtf(disc)) / 2.0f;

	if (d0 > d1) {
		float aux = d0;
		d0 = d1;
		d1 = aux;
	}

	if (d1 < 0.0f) return false;

	t1 = fmax(d0, 0.0f) * rayD + p1;
	t2 = (d1 > vmath::distance(p1, p2)) ? p2 : d1 * rayD + p1;

	return true;
}