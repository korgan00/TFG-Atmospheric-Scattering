#ifndef OBJ_LOADER_H_
#define OBJ_LOADER_H_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;
#include "../../common/common.h"


class ObjLoader {
public:

	typedef struct {
		vmath::ivec3 v;
		vmath::ivec3 vt;
		vmath::ivec3 vn;
		GLint material;
	} Face3v;
	typedef struct {
		Face3v *faces;
		GLint count;
	} Faces3v;

	typedef struct {
		Faces3v faces;
		std::string name;
	} ObjectGroup;

	typedef struct {
		Faces3v faces;
		vector<ObjectGroup*> groups;
		std::string name;
	} NamedObject;

	typedef struct {
		Vertices v;
		Normals vn;
		TexVertices vt;
		Materials mtl;
		vector<NamedObject*> namedObjects;
	} ObjFileInfo;

private:
	static void count(ifstream &input, vector<NamedObject*> &namedObjects,
		GLuint* vCount, GLuint* vnCount, GLuint* vtCount, GLuint* matCount);
	static void countMtllib(char buffer[256], GLuint *matCount);
	inline static void initializeSizes(Vertices &v, Normals &vn, TexVertices &vt, Materials &matlibs);
	inline static void initializeNamedObjects(vector<NamedObject*> &namedObjs);
	inline static void initializeObjectGroups(vector<ObjectGroup*> &objGroups);

	static void fillData(ifstream& input, Vertices &v, Normals &vn, TexVertices &vt,
		Materials &matlibs, vector<NamedObject*> &namedObjs);
	inline static void processVertex(char buffer[256], Vertices *v, Normals *vn, TexVertices *vt);
	inline static void processFace(char buffer[256], Faces3v *f, GLint currMat);
	inline static void processMtlLib(char buffer[256], Materials &mtllibs);

	inline static NamedObject* newNamedObject();
	inline static ObjectGroup* newObjectGroup();
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
	static string pathToObjects;
	static ObjFileInfo* load(string file);
};

#endif /* OBJ_LOADER_H_ */