#ifndef OBJ_TO_MESH_H_
#define OBJ_TO_MESH_H_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
#include "../../common/common.h"
#include "ObjLoader.h"
#include "Mesh.h"



class ObjNameToTextureName {
public:
	virtual string getDiffuseTexName(string objName) = 0;
	virtual string getNormalMapTexName(string objName) = 0;
};

class ObjToMesh {

private:

	static GLboolean _useObjNameAsTextureHint;
	static ObjNameToTextureName* _converter;

	static void processFaces(ObjLoader::Faces3v &faces, const ObjLoader::ObjFileInfo &objFile,
		vector<Mesh::PerVertex*> &vertexBufferData, vector<Mesh::PerDraw*> &elementBufferData,
		GLint &lastMaterial, vector<GLuint> &currIndexes, GLuint &currentIndex, string objName);
	static void perVertexFace(Mesh::PerVertex* perVertex[3], const ObjLoader::Face3v &f, const ObjLoader::ObjFileInfo &objFile);
	static void closeIndexBuffer(GLint &lastMaterial, const ObjLoader::ObjFileInfo &objFile,
		vector<GLuint> &currIndexes, vector<Mesh::PerDraw*> &elementBufferData, string objName);
public:
	// infered data
	/*
	typedef struct {
	Vertex v;
	Normal vn;
	TexVertex vt;
	} PerVertex;

	typedef struct {
	PerVertex *v;
	GLuint *indices;
	Material mat;

	GLuint vertexCount;
	GLuint indicesCount;
	} PerDraw;
	*/
	static Mesh* convert(ObjLoader::ObjFileInfo* objFile);
	static Mesh* convert(ObjLoader::ObjFileInfo* objFile, ObjNameToTextureName* converter);

};

#endif /* OBJ_TO_MESH_H_ */