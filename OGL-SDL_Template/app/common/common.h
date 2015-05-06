#ifndef COMMON_H_
#define COMMON_H_

#define DEFAULT_WIN_HEIGHT 900
#define DEFAULT_WIN_WIDTH 1200

#include <string>
#include <GL/gl3w.h>
#include "vmath.h"
#include "LoadShaders.h"
#include "infoManager.h"
#include "Logger.h"
#include <SDL.h>

const std::string SHADERS_DIR = "../OGL-SDL_Template/app/shaders/";

typedef vmath::vec3 Vertex;
typedef struct {
	Vertex *vertices;
	GLuint count;
} Vertices;

typedef vmath::vec3 Normal;
typedef struct {
	Normal *normals;
	GLuint count;
} Normals;

typedef vmath::vec2 TexVertex;
typedef struct {
	TexVertex *texVertices;
	GLuint count;
} TexVertices;

typedef struct {
	std::string name;
	GLfloat alpha;
	vmath::vec3 diffuse;
	vmath::vec3 ambient;
	vmath::vec3 specular;
	GLfloat specularExponent;
	GLfloat opticalDensity;

	GLint illumination;
	string textureDiffuse;
	string textureNormalMap;
} Material;
typedef struct {
	Material *materials;
	GLuint count;
} Materials;

void CheckGLErr(int line, char* fileName);

#define CheckErr() CheckGLErr(__LINE__, __FILE__)

#endif