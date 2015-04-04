#include "ObjToMesh.h"

GLboolean ObjToMesh::_useObjNameAsTextureHint = GL_FALSE;
ObjNameToTextureName* ObjToMesh::_converter = nullptr;

Mesh* ObjToMesh::convert(ObjLoader::ObjFileInfo *objFile, ObjNameToTextureName* converter) {
	ObjToMesh::_useObjNameAsTextureHint = GL_TRUE;
	ObjToMesh::_converter = converter;
	Mesh *m = convert(objFile);
	ObjToMesh::_useObjNameAsTextureHint = GL_FALSE;
	return m;
}

Mesh* ObjToMesh::convert(ObjLoader::ObjFileInfo *objFile) {
	
	typedef vector<ObjLoader::NamedObject*>::iterator NamedObjIt;
	typedef vector<ObjLoader::ObjectGroup*>::iterator ObjGroupIt;
	vector<ObjLoader::NamedObject*> objs = objFile->namedObjects;

	vector<Mesh::PerVertex*> vertexBufferData;
	vector<Mesh::PerDraw*> elementBufferData;
	vector<GLuint> currIndexes;
	
	GLuint currentIndex = 0;
	GLint lastMaterial = -1;

	for (NamedObjIt o = objs.begin(); o != objs.end(); ++o) {
		ObjLoader::Faces3v faces = (*o)->faces;
		processFaces(faces, *objFile, vertexBufferData, elementBufferData, 
			lastMaterial, currIndexes, currentIndex, (*o)->name);
		closeIndexBuffer(lastMaterial, *objFile, currIndexes, elementBufferData, (*o)->name);

		for (ObjGroupIt g = (*o)->groups.begin(); g != (*o)->groups.end(); ++g) {
			ObjLoader::Faces3v faces = (*g)->faces;
			processFaces(faces, *objFile, vertexBufferData, elementBufferData, 
				lastMaterial, currIndexes, currentIndex, (*o)->name);
			closeIndexBuffer(lastMaterial, *objFile, currIndexes, elementBufferData, (*g)->name);
		}
	}

	GLuint vCount = vertexBufferData.size();
	GLuint eCount = elementBufferData.size();
	Mesh::PerVertex* vertexBufferDataArray = new Mesh::PerVertex[vCount];
	Mesh::PerDraw* elementBufferDataArray = new Mesh::PerDraw[eCount];
	GLuint i = 0;
	for (vector<Mesh::PerVertex*>::iterator perV = vertexBufferData.begin(); perV != vertexBufferData.end(); ++perV) {
		vertexBufferDataArray[i++] = **perV;
	}
	i = 0;
	for (vector<Mesh::PerDraw*>::iterator perD = elementBufferData.begin(); perD != elementBufferData.end(); ++perD) {
		elementBufferDataArray[i++] = **perD;
	}

	return new Mesh(vertexBufferDataArray, vCount, elementBufferDataArray, eCount);
}

void ObjToMesh::processFaces(ObjLoader::Faces3v &faces, const ObjLoader::ObjFileInfo &objFile, 
			vector<Mesh::PerVertex*> &vertexBufferData, vector<Mesh::PerDraw*> &elementBufferData, 
			GLint &lastMaterial, vector<GLuint> &currIndexes, GLuint &currentIndex, string objName) {

	for (int i = 0; i < faces.count; i++) {
		Mesh::PerVertex* perVertex[3];
		perVertexFace(perVertex, faces.faces[i], objFile);

		vertexBufferData.push_back(perVertex[0]);
		vertexBufferData.push_back(perVertex[1]);
		vertexBufferData.push_back(perVertex[2]);


		if (faces.faces[i].material == lastMaterial) {
			currIndexes.push_back(currentIndex++);
			currIndexes.push_back(currentIndex++);
			currIndexes.push_back(currentIndex++);
		} else {
			closeIndexBuffer(lastMaterial, objFile, currIndexes, elementBufferData, objName);
			lastMaterial = faces.faces[i].material;
		}
	}
}

void ObjToMesh::closeIndexBuffer(GLint &lastMaterial, const ObjLoader::ObjFileInfo &objFile, 
		vector<GLuint> &currIndexes, vector<Mesh::PerDraw*> &elementBufferData, string objName) {

	if (currIndexes.size() > 0) {

		Material *mtl = lastMaterial != -1 ? &objFile.mtl.materials[lastMaterial] : nullptr;
		if (mtl == nullptr && ObjToMesh::_useObjNameAsTextureHint == GL_TRUE) {
			mtl = new Material();
			mtl->textureDiffuse = ObjToMesh::_converter->getDiffuseTexName(objName);
			mtl->alpha = 1;
			mtl->ambient = vmath::vec3(0.0f, 0.0f, 0.0f);
			mtl->specular = vmath::vec3(0.6f, 0.6f, 0.6f);
			mtl->diffuse = vmath::vec3(0.0f, 0.0f, 0.0f);
			mtl->illumination = 1;
			mtl->name = "";
			mtl->opticalDensity = 0;
			mtl->specularExponent = 1.0f;
		}

		GLuint *indices = new GLuint[currIndexes.size()];
		GLuint j = 0;
		for (std::vector<GLuint>::iterator idx = currIndexes.begin(); idx != currIndexes.end(); ++idx) {
			indices[j++] = *idx;
		}
		Mesh::PerDraw *perDraw = new Mesh::PerDraw({ mtl, indices, currIndexes.size() });
		elementBufferData.push_back(perDraw);

		currIndexes.clear();

	}
}

void ObjToMesh::perVertexFace(Mesh::PerVertex* perVertex[3], const ObjLoader::Face3v &f, const ObjLoader::ObjFileInfo &objFile) {
	Vertex v_a, v_b, v_c;
	Normal vn_a, vn_b, vn_c;
	TexVertex vt_a, vt_b, vt_c;

	// PER VERTEX
	v_a = objFile.v.vertices[f.v[0] - 1];
	v_b = objFile.v.vertices[f.v[1] - 1];
	v_c = objFile.v.vertices[f.v[2] - 1];

	if (f.vn[0] != -1) {
		vn_a = objFile.vn.normals[f.vn[0] - 1];
		vn_b = objFile.vn.normals[f.vn[1] - 1];
		vn_c = objFile.vn.normals[f.vn[2] - 1];
	} else {
		vn_a = vn_b = vn_c = Normal({0, 0, 0});
	}

	if (f.vt[0] != -1) {
		vt_a = objFile.vt.texVertices[f.vt[0] - 1];
		vt_b = objFile.vt.texVertices[f.vt[1] - 1];
		vt_c = objFile.vt.texVertices[f.vt[2] - 1];
	} else {
		vt_a = vt_b = vt_c = TexVertex({-1, -1});
	}

	perVertex[0] = new Mesh::PerVertex({ v_a, vn_a, vt_a });
	perVertex[1] = new Mesh::PerVertex({ v_b, vn_b, vt_b });
	perVertex[2] = new Mesh::PerVertex({ v_c, vn_c, vt_c });
}


