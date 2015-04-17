#include "Shader.h"



void Shader::scatteringVariables(ScatteringUniformPseudoConstants_values scattValues) {
	/*vmath::vec3 lightDir, GLfloat lightSun, vmath::vec3 betaER,
	vmath::vec3 betaEM, vmath::vec3 betaSR, vmath::vec3 betaSM) {*/
	//glUniformMatrix4fv(_scatteringUniforms.density, 1, GL_FALSE, *density);

	glUniform3fv(_SUids.lightDir, 1, scattValues.lightDir);
	glUniform1f(_SUids.lightSun, scattValues.lightSun);

	glUniform3fv(_SUids.betaER, 1, scattValues.betaER);
	glUniform3fv(_SUids.betaEM, 1, scattValues.betaEM);
	glUniform3fv(_SUids.betaSR, 1, scattValues.betaSR);
	glUniform3fv(_SUids.betaSM, 1, scattValues.betaSM);
}

void Shader::scatteringConstants(ScatteringUniformConstants_values scattValues) {
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
	CheckErr();


	SDL_Surface *texR = nullptr, *texM = nullptr;
	createHeightScatterMap(scattValues, texR, texM);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _tso[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texR->w, texR->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texR->pixels);

	CheckErr();



	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _tso[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texM->w, texM->h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texM->pixels);
	CheckErr();

	//IMG_SavePNG(texM, "C:/Users/MisiKorgan/Desktop/pruebaScatterMapMie.png");
	//IMG_SavePNG(texR, "C:/Users/MisiKorgan/Desktop/pruebaScatterMapRay.png");
}


void Shader::createHeightScatterMap(ScatteringUniformConstants_values scattValues, SDL_Surface* &texR, SDL_Surface* &texM) {

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

	// Hay que generalizarlo
	GLuint accum = 0;
	for (GLint w = 0; w < texR->w; w += 1) {
		GLfloat initialHeight = ((atmRadius - earthRadius) * w) / texR->w;
		GLfloat point_earth = initialHeight + earthRadius;
		vmath::vec2 point(0.0f, point_earth);

		for (GLint h = 0; h < texR->h; h += 1) {
			GLfloat cosPhi = (2.0f * h / (texR->h - 1)) - 1.0f;
			GLfloat senPhi = sqrt(1.0f - cosPhi*cosPhi);

			vmath::vec2 normLightDir(senPhi, -cosPhi);

			GLfloat b = 2 * point_earth * -cosPhi;
			GLfloat c = point_earth * point_earth - atmRadius2;
			GLfloat sqrt_bb_4c = sqrt(b*b - 4 * c);
			GLfloat a1 = (b + sqrt_bb_4c) / 2;
			GLfloat a2 = (b - sqrt_bb_4c) / 2;
			
			GLfloat diferential_A = fmax(a1, a2) / STEPS;
			vmath::vec2 delta_A(-normLightDir * diferential_A);

			GLfloat density_AP[2] = { 0.0, 0.0 };
			accum++;
			for (GLfloat step = 0.5f; step < STEPS; step += 1.0f) {
				GLfloat hPoint = vmath::distance(vmath::vec2(0.0f, 0.0f), point + delta_A * step) - earthRadius;
				
					GLfloat relation[2] = { -hPoint / H_R, -hPoint / H_M };
					density_AP[0] += P0 * exp(relation[0]) * diferential_A;
					density_AP[1] += P0 * exp(relation[1]) * diferential_A;
				
			}
			pixR[(h * texR->w) + w] = (Uint32)density_AP[0];
			pixM[(h * texR->w) + w] = (Uint32)density_AP[1];
		}
	}


	Uint32 myuint = pixR[(490 * texR->w) + 6];
	float r, g, b, a;
	r = (myuint % 255) / 255.f;
	g = ((myuint >> 8) % 255) / 255.f;
	b = ((myuint >> 16) % 255) / 255.f;
	a = ((myuint >> 24) % 255) / 255.f;
	vmath::vec4 abgr(a, b, g, r);
	vmath::vec4 vec(256 * 256 * 256, 256 * 256, 256, 1);
	float pepe = vmath::dot(abgr, vec) * 255;
	std::cout << myuint << " :::: " << pepe << endl;

	for (int i = 0; i < 500; i+= 25) {
		std::cout << pixR[(490 * texR->w) + i] << endl;
	}
	
}